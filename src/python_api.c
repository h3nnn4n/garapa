#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "assert.h"
#include "config.h"
#include "memory.h"
#include "python_api.h"
#include "types.h"

static _context *current_context = NULL;
static PyObject *vblank_callback = NULL;
static int       user_input_enabled = 1;

static PyMethodDef EmbMethods[] = {
    {"hello_world", garapa_hello_world, METH_VARARGS, "Return a garapa greeting."},
    {"peek", garapa_peek, METH_VARARGS, "Peeking into the memory"},
    {"set_vblank_callback", garapa_set_vblank_callback, METH_VARARGS, "Sets a callback to be run on vblank"},
    {"enable_user_input", garapa_enable_user_input, METH_VARARGS, "Allows user input to pe processes"},
    {"disable_user_input", garapa_disable_user_input, METH_VARARGS, "Prevents user input from being processed"},
    {"set_input", garapa_set_input, METH_VARARGS, "Set the console input keys register"},
    {"kill_emulator", garapa_kill_emulator, METH_VARARGS, "Kill the emulator"},
    {NULL, NULL, 0, NULL},
};

static PyModuleDef EmbModule = {PyModuleDef_HEAD_INIT, "garapa", NULL, -1, EmbMethods, NULL, NULL, NULL, NULL};

static PyObject *PyInit_garapa(void) { return PyModule_Create(&EmbModule); }

void set_current_context(_context *context) { current_context = context; }

PyObject *garapa_enable_user_input(__attribute__((unused)) PyObject *self, __attribute__((unused)) PyObject *args) {
    user_input_enabled = 1;
    Py_RETURN_NONE;
}

PyObject *garapa_disable_user_input(__attribute__((unused)) PyObject *self, __attribute__((unused)) PyObject *args) {
    user_input_enabled = 0;
    Py_RETURN_NONE;
}

PyObject *garapa_kill_emulator(__attribute__((unused)) PyObject *self, __attribute__((unused)) PyObject *args) {
    exit(0);
}

PyObject *garapa_hello_world(__attribute__((unused)) PyObject *self, __attribute__((unused)) PyObject *args) {
    return Py_BuildValue("s", "hello world, garapa is tasty");
}

PyObject *garapa_set_vblank_callback(__attribute__((unused)) PyObject *self, __attribute__((unused)) PyObject *args) {
    assert(current_context != NULL && "Current context is NULL!! Good luck peeking into that");

    PyObject *temp;

    if (!PyArg_ParseTuple(args, "O", &temp)) {
        fprintf(stderr, "failed to parse args\n");
        PyErr_SetString(PyExc_TypeError, "failed to parse args");
        return NULL;
    }

    if (!PyCallable_Check(temp)) {
        fprintf(stderr, "parameter is not callable\n");
        PyErr_SetString(PyExc_TypeError, "parameter is not callable");
        return NULL;
    }

    // Decrease the ref count of the old callback, if any
    if (vblank_callback != NULL)
        Py_XDECREF(vblank_callback);

    vblank_callback = temp;
    Py_XINCREF(vblank_callback);

    Py_RETURN_NONE;
}

PyObject *garapa_peek(__attribute__((unused)) PyObject *self, PyObject *args) {
    assert(current_context != NULL && "Current context is NULL!! Good luck peeking into that");

    if (current_context == NULL) {
        printf("Warning, current_context is NULL!\n");
        return NULL;
    }

    long py_addr_value;
    if (!PyArg_ParseTuple(args, "i", &py_addr_value)) {
        fprintf(stderr, "failed to parse args!\n");
    }

    uint16_t addr  = (uint16_t)py_addr_value;
    uint8_t  value = read_byte(current_context->cpu_info, addr);

    return PyLong_FromLong(value);
}

PyObject *garapa_set_input(__attribute__((unused)) PyObject *self, PyObject *args) {
    assert(current_context != NULL && "Current context is NULL!! Good luck peeking into that");

    long  key_state;
    char *key_name;

    if (!PyArg_ParseTuple(args, "sd", &key_name, &key_state)) {
        fprintf(stderr, "failed to parse args!\n");
    }

    if (strcmp(key_name, "right") == 0) {
        current_context->cpu_info->joystick.button_right = !key_state;
    } else if (strcmp(key_name, "left") == 0) {
        current_context->cpu_info->joystick.button_left = !key_state;
    } else if (strcmp(key_name, "up") == 0) {
        current_context->cpu_info->joystick.button_up = !key_state;
    } else if (strcmp(key_name, "down") == 0) {
        current_context->cpu_info->joystick.button_down = !key_state;
    } else if (strcmp(key_name, "a") == 0) {
        current_context->cpu_info->joystick.button_a = !key_state;
    } else if (strcmp(key_name, "b") == 0) {
        current_context->cpu_info->joystick.button_b = !key_state;
    } else if (strcmp(key_name, "select") == 0) {
        current_context->cpu_info->joystick.button_select = !key_state;
    } else if (strcmp(key_name, "start") == 0) {
        current_context->cpu_info->joystick.button_start = !key_state;
    } else {
        fprintf(stderr, "Got invalid key press: %s\nAborting\n", key_name);
        exit(-1);
    }

    if (PyErr_Occurred()) {
        fprintf(stderr, "An error occurred when running the python file. Aborting\n");
        exit(-1);
    }

    Py_RETURN_NONE;
}

int is_user_input_enabled() { return user_input_enabled; }

void py_init(__attribute__((unused)) int argc, char **argv) {
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        return;
    }

    wchar_t **wargv = PyMem_Malloc(sizeof(wchar_t *) * argc);
    for (int i = 0; i < argc; i++) {
        wchar_t *arg = Py_DecodeLocale(argv[i], NULL);
        wargv[i]     = arg;
    }

    PyObject *pName, *pModule, *pFunc;

    PyImport_AppendInittab("garapa", &PyInit_garapa);
    Py_SetProgramName(program);
    Py_Initialize();
    PySys_SetArgv(argc, wargv);

    pName   = PyUnicode_DecodeFSDefault(get_config_value_s("python_filename"));
    pModule = PyImport_Import(pName);

    Py_DECREF(pName);

    if (pModule != NULL) {
        char *fname = "main";

        if (get_config_value_s("python_function") != NULL)
            fname = get_config_value_s("python_function");

        pFunc = PyObject_GetAttrString(pModule, fname);

        if (pFunc && PyCallable_Check(pFunc)) {
            PyObject_CallObject(pFunc, NULL);
        } else {
            if (PyErr_Occurred())
                PyErr_Print();

            fprintf(stderr, "Failed to call \"%s\"\n", fname);
        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
    }

    if (PyErr_Occurred()) {
        fprintf(stderr, "An error occurred when running the python file. Aborting\n");
        PyErr_Print();
        exit(-1);
    }
}

void trigger_vblank_callback() {
    if (vblank_callback == NULL)
        return;

    assert(PyCallable_Check(vblank_callback) && "Callback must be callable");
    PyObject_CallObject(vblank_callback, NULL);

    if (PyErr_Occurred()) {
        PyErr_Print();
        fprintf(stderr, "Got a python exception. Aborting\n");
        exit(-1);
    }
}
