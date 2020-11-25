#pragma once

#include <Python.h>

#include "types.h"

void      py_init(int argc, char **argv);
void      set_current_context(_context *context);
PyObject *garapa_hello_world(PyObject *self, PyObject *args);
PyObject *garapa_peek(PyObject *self, PyObject *args);
PyObject *garapa_set_vblank_callback(PyObject *self, PyObject *args);
PyObject *garapa_enable_user_input(PyObject *self, PyObject *args);
PyObject *garapa_disable_user_input(PyObject *self, PyObject *args);
PyObject *garapa_set_input(PyObject *self, PyObject *args);

void trigger_vblank_callback();
int  is_user_input_enabled();
