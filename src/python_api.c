#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "python_api.h"

static PyObject* garapa_hello_world(__attribute__((unused)) PyObject *self, __attribute__((unused)) PyObject *args) {
  return Py_BuildValue("s", "hello world, garapa is tasty");
}

static PyMethodDef EmbMethods[] = {
  {"hello_world", garapa_hello_world, METH_VARARGS, "Return a garapa greeting."},
  {NULL, NULL, 0, NULL}
};

static PyModuleDef EmbModule = {
  PyModuleDef_HEAD_INIT, "garapa", NULL, -1, EmbMethods,
  NULL, NULL, NULL, NULL
};

static PyObject* PyInit_garapa(void) {
  return PyModule_Create(&EmbModule);
}

void py_init(__attribute__((unused)) int argc, char **argv) {
  wchar_t *program = Py_DecodeLocale(argv[0], NULL);
  if (program == NULL) {
    fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
    return;
  }

  wchar_t **wargv = PyMem_Malloc(sizeof(wchar_t*) * argc);
  for (int i = 0; i < argc; i++) {
    wchar_t *arg = Py_DecodeLocale(argv[i], NULL);
    wargv[i] = arg;
  }

  PyObject *pName, *pModule, *pFunc;

  PyImport_AppendInittab("garapa", &PyInit_garapa);
  Py_SetProgramName(program);
  Py_Initialize();
  PySys_SetArgv(argc, wargv);

  pName = PyUnicode_DecodeFSDefault("main");
  pModule = PyImport_Import(pName);

  Py_DECREF(pName);

  if (pModule != NULL) {
    pFunc = PyObject_GetAttrString(pModule, "main");

    if (pFunc && PyCallable_Check(pFunc)) {
      PyObject_CallObject(pFunc, NULL);
    } else {
      if (PyErr_Occurred()) PyErr_Print();

      fprintf(stderr, "Failed to call \"%s\"\n", "main");
    }

    Py_XDECREF(pFunc);
    Py_DECREF(pModule);
  }

  if (Py_FinalizeEx() < 0) {
    fprintf(stderr, "Py_FinalizeEx returned an error");
    exit(-1);
  }
}
