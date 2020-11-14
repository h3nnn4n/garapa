#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "python_api.h"

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
