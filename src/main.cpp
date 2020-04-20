#include "planets.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Local Headers
#include "scene.hpp"
#include "utils/window_size.hpp"
#include "utils/input/key_input.hpp"
#include "utils/input/mouse_input.hpp"

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    // Initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    GLFWwindow * mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // glEnable(GL_DEBUG_OUTPUT);
    // glDebugMessageCallback(glMessageCallback, NULL); 

    WindowSize::setInputWindow(mWindow);
    KeyInput::setInputWindow(mWindow);
    MouseInput::setInputWindow(mWindow);

    // Initialize OpenGL
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.01f, 0.01f, .01f, 1.0f);  

    double prev_time = glfwGetTime();
    int frame_count = 0;
    double remaining_second = 1;

    Scene * scene = new Scene(); 

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double current_time = glfwGetTime();
        double delta_time = current_time - prev_time;

        frame_count++;

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
        }

        scene->draw(delta_time);
        KeyInput::update();
        MouseInput::update();

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        prev_time = current_time;
    }   glfwTerminate();
    return EXIT_SUCCESS;
}