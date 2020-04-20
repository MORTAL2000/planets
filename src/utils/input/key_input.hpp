

#pragma once

// System Headers
#include <GLFW/glfw3.h>

namespace KeyInput
{
    void setInputWindow(GLFWwindow* window);

    void update();

    bool justPressed(int key);

    bool pressed(int key);

    bool justReleased(int key);
}