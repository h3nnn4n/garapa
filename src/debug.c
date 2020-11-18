#include <stdio.h>
#include <stdlib.h>

#include "debug.h"


void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                const GLchar *message, __attribute__((unused)) const void *userParam) {
    int is_error = type == GL_DEBUG_TYPE_ERROR;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s, source = %d, id = %d, length = %d\n",
            (is_error ? "** GL ERROR **" : ""), type, severity, message, source, id, length);

    if (is_error) {
        fprintf(stderr, "Found openGL error. Aborting!\n");
        abort();
    }
}
