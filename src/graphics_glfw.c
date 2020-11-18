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
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include <stb_image.h>

#include "graphics_glfw.h"
#include "debug.h"
#include "glfw_input_handling.h"
#include "graphics.h"
#include "shader_c.h"
#include "types.h"

static uint32_t *pixels = NULL;

#if defined(USE_GLFW)
static const int scale             = 4;
static const int PPU_SCREEN_WIDTH  = 160;
static const int PPU_SCREEN_HEIGHT = 144;
static const int WINDOW_WIDTH      = PPU_SCREEN_HEIGHT * scale;
static const int WINDOW_HEIGHT     = PPU_SCREEN_HEIGHT * scale;

GLFWwindow * window;
Shader *     shader;
unsigned int VBO, VAO, EBO;
unsigned int texture1;

void build_quad() {
    float vertices[] = {
        // positions          // colors           // texture coords
        1.0f,  1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void build_texture() {
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

int glfw_init() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetWindowAspectRatio(window, PPU_SCREEN_WIDTH, PPU_SCREEN_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    assert((sizeof(uint32_t) * PPU_SCREEN_WIDTH * PPU_SCREEN_HEIGHT) > 0 &&
           "??? trying to allocate no pixels??? Invalid screen size???");
    pixels = malloc(sizeof(uint32_t) * PPU_SCREEN_WIDTH * PPU_SCREEN_HEIGHT);
    assert(pixels != NULL && "failed to allocate pixel matrix for the display");
    memset(pixels, 255, sizeof(uint32_t) * PPU_SCREEN_WIDTH * PPU_SCREEN_HEIGHT);

    shader = newShader("shaders/shader.vert", "shaders/shader.frag", NULL);
    build_quad();
    build_texture();

    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FRAMEBUFFER_SRGB);

    Shader_use(shader);
    Shader_set_int(shader, "texture_1", 0);

    return 0;
}

void flip_screen_glfw(__attribute__((unused)) _cpu_info *cpu) {
    if (glfwWindowShouldClose(window))
        exit(0);
    if (glfwGetCurrentContext() != window)
        glfwMakeContextCurrent(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader_use(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, PPU_SCREEN_WIDTH, PPU_SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void input_update_glfw(_cpu_info *cpu) {
    glfwPollEvents();
    process_input_glfw(window, cpu);
}

void glfw_exit() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    Shader_destroy(shader);
    glfwTerminate();
}

#else

int  glfw_init() { return 0; }
void glfw_exit() {}
void flip_screen_glfw(__attribute__((unused)) _cpu_info *cpu) {}
void input_update_glfw(__attribute__((unused)) _cpu_info *cpu) {}

#endif

uint32_t *get_glfw_frame_buffer() { return pixels; }
