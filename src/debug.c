#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  int is_error = type == GL_DEBUG_TYPE_ERROR;
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
         ( is_error ? "** GL ERROR **" : "" ),
         type, severity, message );

  if (is_error) {
    abort();
  }
}
