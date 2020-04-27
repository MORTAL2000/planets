

#pragma once

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace WindowSize
{
    extern int width, height, widthPixels, heightPixels;
    extern bool should_resize;

    void setInputWindow(GLFWwindow* window);
    void resize();
}
