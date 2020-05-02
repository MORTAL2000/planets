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

#include "graphics/imgui/imgui.h"
#include "graphics/imgui/imgui_impl_glfw.h"
#include "graphics/imgui/imgui_impl_opengl3.h"

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

static void ShowOverlap(const char * text)
{
    const float DISTANCE = 10.0f;
    static int corner = 3;
    ImGuiIO& io = ImGui::GetIO();
    if (corner != -1)
    {
        ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    
    if (ImGui::Begin("Example: Simple overlay", NULL, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::Text(text);
        ImGui::Separator();
        if (ImGui::IsMousePosValid())
            ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
            ImGui::Text("Mouse Position: <invalid>");
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
            if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
            if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

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
    
    mWindow = glfwCreateWindow(mWidth, mHeight, "Planets - Alex Kafer CSCI5611 Final Project", nullptr, nullptr);

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ImGui::StyleColorsClassic();
    
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

    std::string fps = "";
    
    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        // Clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // feed inputs to dear imgui, start new frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        double current_time = glfwGetTime();
        double delta_time = current_time - prev_time;

        frame_count++;
        
        check_gl_error();

        if ((remaining_second -= delta_time) <= 0)
        {
            fps = std::to_string(frame_count) + "fps";
            frame_count = 0;
            remaining_second = 1;
        }

         ShowOverlap(fps.c_str());

        if (WindowSize::should_resize)
        {
            WindowSize::resize();
            Globals::scene->resize();
        }

        Globals::scene->draw(min(delta_time, .1));
        check_gl_error();

        // ImGui::ShowDemoWindow();

        KeyInput::update();
        MouseInput::update();

        // Render dear imgui into screen
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();

        prev_time = current_time;
    }   glfwTerminate();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return EXIT_SUCCESS;
}
