/******************************************************************************
 * Copyright (C) 2020  Renan S. Silva                                         *
 *                                                                            *
 * This software is provided 'as-is', without any express or implied          *
 * warranty. In no event will the authors be held liable for any damages      *
 * arising from the use of this software.                                     *
 *                                                                            *
 * Permission is granted to anyone to use this software for any purpose,      *
 * including commercial applications, and to alter it and redistribute it     *
 * freely, subject to the following restrictions:                             *
 *                                                                            *
 * 1. The origin of this software must not be misrepresented; you must not    *
 *    claim that you wrote the original software. If you use this software    *
 *    in a product, an acknowledgment in the product documentation would be   *
 *    appreciated but is not required.                                        *
 * 2. Altered source versions must be plainly marked as such, and must not be *
 *    misrepresented as being the original software.                          *
 * 3. This notice may not be removed or altered from any source distribution. *
 ******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "types.h"
#include "overlay.h"
#include "graphics.h"
#include "glfw_input_handling.h"

GLFWwindow *window;

static uint32_t *pixels = NULL;

static const int WINDOW_WIDTH = 160;
static const int WINDOW_HEIGHT = 144;

uint32_t *get_glfw_frame_buffer () {
  /*return pixels;*/
  return NULL;
}

int glfw_init () {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Here comes dat gameboi", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr, "Failed to create GLFW window\n");
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return -1;
  }

  pixels = malloc ( sizeof ( uint32_t ) * WINDOW_HEIGHT * WINDOW_WIDTH);
  memset(pixels, 255, WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

  glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_FRAMEBUFFER_SRGB);

  return 0;
}

void flip_screen_glfw ( _cpu_info *cpu ) {
  glfwSwapBuffers(window);

  if (glfwWindowShouldClose(window)) exit(0);
}

void input_update_glfw ( _cpu_info *cpu ) {
  glfwPollEvents();
  process_input_glfw(window, cpu);
}

void glfw_exit() {
  glfwTerminate();
}
