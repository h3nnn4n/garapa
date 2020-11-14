#define PY_SSIZE_T_CLEAN

#include <Python.h>

#include "python_api.h"

void py_init(__attribute__((unused)) int argc, char **argv) {
  wchar_t *program = Py_DecodeLocale(argv[0], NULL);
  if (program == NULL) {
    fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
    exit(1);
  }
  Py_SetProgramName(program);

  Py_Initialize();

  PyRun_SimpleString("print('hello python world')");

  if (Py_FinalizeEx() < 0) {
    fprintf(stderr, "Py_FinalizeEx returned an error");
  }
}
