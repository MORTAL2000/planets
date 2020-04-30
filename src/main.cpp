#include "planets.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS //ensure we are using radians

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Local Headers
#include "scene.hpp"
#include "graphics/gl_error.hpp"
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"

namespace Globals {
    Scene * scene;
}

static void error_callback(int error, const char* description){ fprintf(stderr, "Error: (%d)%s\n", error, description); }

// static void glfwErrorCallback(int, const char *message) {
//   fprintf(stderr, "%s\n", message);
// }

// static void glMessageCallback(GLenum, GLenum, GLuint, GLenum, GLsizei,
//                               const GLchar *message, const void *) {
//   glfwErrorCallback(0, message);
// }

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    GLFWwindow * mWindow;
    glfwSetErrorCallback(&error_callback);
    // Initialise GLFW
    
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        std::cerr << "Failed to Create OpenGL Context" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);

    // glDebugMessageCallback(glMessageCallback, NULL);
    
    // Initialize OpenGL
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    
    WindowSize::setInputWindow(mWindow);
    KeyInput::setInputWindow(mWindow);
    MouseInput::setInputWindow(mWindow);
    
    check_gl_error();

    double prev_time = glfwGetTime();
    int frame_count = 0;
    double remaining_second = 1;

    Globals::scene = new Scene();
    check_gl_error();

    WindowSize::resize();
    Globals::scene->resize();
    check_gl_error();
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Clear screen
        // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        double delta_time = current_time - prev_time;

        frame_count++;
        
        check_gl_error();

        if ((remaining_second -= delta_time) <= 0)
        {
            std::string fps = std::to_string(frame_count) + "fps";
            glfwSetWindowTitle(mWindow, fps.c_str());
            frame_count = 0;
            remaining_second = 1;
        }

        if (WindowSize::should_resize)
        {
            WindowSize::resize();
            Globals::scene->resize();
        }

        Globals::scene->draw(min(delta_time, .1));
        check_gl_error();

        KeyInput::update();
        MouseInput::update();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        prev_time = current_time;
    }   glfwTerminate();
    return EXIT_SUCCESS;
}
