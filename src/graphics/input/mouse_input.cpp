
#include "mouse_input.hpp"
#define NR_OF_BUTTONS 8

#include "graphics/imgui/imgui.h"

namespace MouseInput
{

namespace
{

GLFWwindow *window;

enum ButtonStatus
{
    NOT_PRESSED,
    JUST_PRESSED,
    PRESSED,
    JUST_RELEASED
};

ButtonStatus buttonStatuses[NR_OF_BUTTONS];

void glfwButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    buttonStatuses[button] = action == GLFW_PRESS ? JUST_PRESSED : JUST_RELEASED;
}

double nextXScroll = 0, nextYScroll = 0, nextMouseX = 0, nextMouseY = 0;

void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    nextXScroll += xoffset;
    nextYScroll += yoffset;
}

void glfwMousePosCallback(GLFWwindow* window, double xpos, double ypos)
{
    nextMouseX = xpos;
    nextMouseY = ypos;
}

static void ShowDebugWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Mouse Input", p_open))
    {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
    ImGui::Columns(2);
    ImGui::Separator();

    struct funcs
    {
        static void ShowKeyValue(const char* key, double value) {
//            ImGui::PushID(kwy);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%s", key);
            ImGui::NextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::Text("%f", value);
            ImGui::NextColumn();

            // if (node_open)
            // {
            //     static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
            //     for (int i = 0; i < 8; i++)
            //     {
            //         ImGui::PushID(i); // Use field index as identifier.
            //         if (i < 2)
            //         {
            //             ShowDummyObject("Child", 424242);
            //         }
            //         else
            //         {
            //             // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
            //             ImGui::AlignTextToFramePadding();
            //             ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
            //             ImGui::NextColumn();
            //             ImGui::SetNextItemWidth(-1);
            //             if (i >= 5)
            //                 ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
            //             else
            //                 ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
            //             ImGui::NextColumn();
            //         }
            //         ImGui::PopID();
            //     }
            //     ImGui::TreePop();
            // }
        }
    };

    funcs::ShowKeyValue("mouseX", mouseX);
    funcs::ShowKeyValue("mouseY", mouseY);

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}


} // namespace

double xScroll = 0, yScroll = 0, mouseX = 0, mouseY = 0, deltaMouseX = 0, deltaMouseY = 0;

bool debugWindow = true;

void setInputWindow(GLFWwindow *inputWindow)
{
    window = inputWindow;

    glfwSetMouseButtonCallback(window, glfwButtonCallback);
    glfwSetScrollCallback(window, glfwScrollCallback);
    glfwSetCursorPosCallback(window, glfwMousePosCallback);

    glfwGetCursorPos(window, &mouseX, &mouseY);
    nextMouseX = mouseX;
    nextMouseY = mouseY;
}

void update()
{
    for (int i = 0; i < NR_OF_BUTTONS; i++)
    {
        ButtonStatus& s = buttonStatuses[i];
        if (s == JUST_PRESSED)
            s = PRESSED;
        else if (s == JUST_RELEASED)
            s = NOT_PRESSED;
    }
    xScroll = nextXScroll;
    yScroll = nextYScroll;
    nextXScroll = nextYScroll = 0;
    
    deltaMouseX = nextMouseX - mouseX;
    deltaMouseY = nextMouseY - mouseY;
    mouseX = nextMouseX;
    mouseY = nextMouseY;
    // dont set nextMouse... to 0 here.

    if (debugWindow) ShowDebugWindow(&debugWindow);
}

void setLockedMode(bool lockedMode)
{
    glfwSetInputMode(window, GLFW_CURSOR, lockedMode ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void setMousePos(double x, double y)
{
    glfwSetCursorPos(window, x, y);
}

bool justPressed(int button)
{
    return buttonStatuses[button] == JUST_PRESSED;
}

bool pressed(int button)
{
    return buttonStatuses[button] == PRESSED || buttonStatuses[button] == PRESSED;
}

bool justReleased(int button)
{
    return buttonStatuses[button] == JUST_RELEASED;
}

} // namespace MouseInput
