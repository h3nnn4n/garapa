#ifndef __GRAPHICS_GLFW
#define __GRAPHICS_GLFW

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "types.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

uint32_t *get_glfw_frame_buffer();
int       glfw_init();
void      glfw_exit();
void      flip_screen_glfw(_cpu_info *cpu);
void      input_update_glfw(_cpu_info *cpu);

#endif // __GRAPHICS_GLFW
