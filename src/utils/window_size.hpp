

#pragma once

// System Headers
#include <GLFW/glfw3.h>

namespace WindowSize
{
    extern int width = 0, height = 0, widthPixels = 0, heightPixels = 0;
    extern bool should_resize;

    void setInputWindow(GLFWwindow* window);
    void resize();
}