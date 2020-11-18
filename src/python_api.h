#pragma once

#include <Python.h>

void py_init(int argc, char **argv);
void set_current_context(_context *context);
PyObject *garapa_hello_world(PyObject *self, PyObject *args);
PyObject *garapa_peek(PyObject *self, PyObject *args);
PyObject *garapa_set_vblank_callback(PyObject *self, PyObject *args);

void trigger_vblank_callback();
