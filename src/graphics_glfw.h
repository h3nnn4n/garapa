#ifndef __GRAPHICS_GLFW
#define __GRAPHICS_GLFW

#include "types.h"

uint32_t *get_glfw_frame_buffer();
int       glfw_init();
void      glfw_exit();
void      flip_screen_glfw(_cpu_info *cpu);
void      input_update_glfw(_cpu_info *cpu);

#endif // __GRAPHICS_GLFW
