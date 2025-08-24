#ifndef IMGUIIMPL_H
#define IMGUIIMPL_H

#include "Core/Macros.h"
#include "Core/Sprocket.pch"

#include "Events/Event.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

namespace Sprocket {
    
    class SPROCKET_API ImGuiImpl {

    private:

        // Singleton Components
        static ImGuiImpl* s_Instance;
        ImGuiImpl() {}
        ImGuiImpl(const ImGuiImpl&) = delete;
        ImGuiImpl operator=(const ImGuiImpl&) = delete;

        std::vector<std::function<void()>> m_RenderFunctions;
        std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeRenderFunctionSlots;

        static void OnUpdate(float deltaTime);
        static void OnClose();

    public:
        /// @brief Initializes the ImGui implementation. Should be called once during application startup. 
        /// Requires a running native windowing context (GLFW).
        static void Init();
        /// @brief Handles incoming events. Should be registered as a callback to recieve events.
        /// @param event The event the should be handled.
        static void OnEvent(Event& event);

        /// @brief Submits a function to be called during the ImGui render phase.
        static unsigned int SubmitRenderFunction(std::function<void()> renderFunction);
        /// @brief Removes a previously submitted render function.
        static void RemoveRenderFunction(unsigned int id);
    };

}

#endif