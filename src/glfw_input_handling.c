#include "glfw_input_handling.h"
#include "python_api.h"

void process_input_glfw(GLFWwindow *window, _cpu_info *cpu) {
    if (!is_user_input_enabled())
        return;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        cpu->joystick.button_a = 0;
    } else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE) {
        cpu->joystick.button_a = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        cpu->joystick.button_b = 0;
    } else if (glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE) {
        cpu->joystick.button_b = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cpu->joystick.button_start = 0;
    } else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        cpu->joystick.button_start = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cpu->joystick.button_select = 0;
    } else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        cpu->joystick.button_select = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cpu->joystick.button_down = 0;
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
        cpu->joystick.button_down = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cpu->joystick.button_up = 0;
    } else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE) {
        cpu->joystick.button_up = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cpu->joystick.button_left = 0;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
        cpu->joystick.button_left = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cpu->joystick.button_right = 0;
    } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
        cpu->joystick.button_right = 1;
    }
}

void framebuffer_size_callback(__attribute__((unused)) GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}
