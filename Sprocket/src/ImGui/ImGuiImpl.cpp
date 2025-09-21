#include "ImGuiImpl.h"

#include "Events/ApplicationEvent.h"

#include "ThirdParty/GLFW/glfw3.h"
#include "ThirdParty/imgui-docking/imgui.h"
#include "ThirdParty/imgui-docking/backends/imgui_impl_glfw.h"
#include "ThirdParty/imgui-docking/backends/imgui_impl_opengl3.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void ImGuiImpl::Init() {
        if(!s_Instance) {
            s_Instance = new ImGuiImpl();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

            ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true);
            ImGui_ImplOpenGL3_Init("#version 460");

            s_Instance->m_FreeRenderFunctionSlots.push(0);
        }
    }

    void ImGuiImpl::OnEvent(Event& event) {
        switch (event.GetEventType()) {
        case EventType::APP_UPDATE:
            OnUpdate(((ApplicationUpdateEvent&)event).GetDeltaTime());
            break;
        case EventType::APP_SHUTDOWN:
            OnShutdown();
            break;
        default:
            break;
        }
    }

    unsigned int ImGuiImpl::SubmitRenderFunction(std::function<void()> renderFunction) {
        unsigned int id = s_Instance->m_FreeRenderFunctionSlots.top();
        s_Instance->m_FreeRenderFunctionSlots.pop();
        
        if(s_Instance->m_FreeRenderFunctionSlots.empty()) {
            s_Instance->m_FreeRenderFunctionSlots.push(id + 1);
        }

        if(id >= s_Instance->m_RenderFunctions.size()) {
            s_Instance->m_RenderFunctions.push_back(renderFunction);
        } else {
            s_Instance->m_RenderFunctions[id] = renderFunction;
        }

        return id;
    }

    void ImGuiImpl::RemoveRenderFunction(unsigned int id) {
        if(id < s_Instance->m_RenderFunctions.size()) {
            s_Instance->m_RenderFunctions[id] = []() {};
            s_Instance->m_FreeRenderFunctionSlots.push(id);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void ImGuiImpl::OnUpdate(float deltaTime) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGuiIO& io = ImGui::GetIO();
        io.DeltaTime = deltaTime;

        for (auto& renderFunction : s_Instance->m_RenderFunctions) {
            renderFunction();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }
    }

    void ImGuiImpl::OnShutdown() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        delete s_Instance;
        s_Instance = nullptr;
    }

}