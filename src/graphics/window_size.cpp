#include "window_size.hpp"

#include <iostream>
namespace WindowSize
{

// size of the window:
// width & height are in screen coordinates, widthPixels & heightPixels are in pixels.
int width = 0, height = 0, widthPixels = 0, heightPixels = 0;
bool should_resize = true;

namespace
{

int nextWidth, nextHeight, nextWidthPixels, nextHeightPixels;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    nextWidth = width;
    nextHeight = height;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    nextWidthPixels = width;
    nextHeightPixels = height;
    should_resize = true;
}

} // namespace

void setInputWindow(GLFWwindow* window)
{
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwGetWindowSize(window, &nextWidth, &nextHeight);
    glfwGetFramebufferSize(window, &nextWidthPixels, &nextHeightPixels);
}

void resize()
{
    WindowSize::width = nextWidth;
    WindowSize::height = nextHeight;
    WindowSize::widthPixels = nextWidthPixels;
    WindowSize::heightPixels = nextHeightPixels;
    WindowSize::should_resize = false;
    glViewport(0, 0, nextWidthPixels, nextHeightPixels);
}

} // namespace WindowSize
