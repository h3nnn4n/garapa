#ifndef _GLFW_INPUT_HANDLING
#define _GLFW_INPUT_HANDLING

#include "types.h"
#include <GLFW/glfw3.h>

void process_input_glfw(GLFWwindow *window, _cpu_info *cpu);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif // _GLFW_INPUT_HANDLING
