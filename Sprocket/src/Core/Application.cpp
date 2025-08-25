#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/EventValidation.h"
#include "Core/Global.h"

#include "Core/Window.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"
#include "Scene/SceneManager.h"
#include "ImGui/ImGuiImpl.h"
#include "Physics/Physics.h"

namespace Sprocket {

    Application::Application() {}
    Application::~Application() {}
    void Application::Start() {}
    void Application::Update(float deltaTime) {}
    void Application::Shutdown(){}

    void Application::Init() {

        if (m_Initialized) {
            return;
        }

        Global::fileLogger.Info("Sprocket: Startup");

        SceneManager::Init(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        this->RegisterEventCallback(SceneManager::OnEvent, EventCategory::UNCATEGORIZED);

        Window::Init(m_WindowDimensions.first, m_WindowDimensions.second, m_WindowTitle);
        Window::RegisterEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        this->RegisterEventCallback(Window::OnEvent, EventCategory::UNCATEGORIZED);

        Input::Init();
        this->RegisterEventCallback(Input::OnEvent, EventCategory::APPLICATION);

        // ImGui must be initialized after the window and much have its callback registered before the renderer
        ImGuiImpl::Init();
        this->RegisterEventCallback(ImGuiImpl::OnEvent, EventCategory::UNCATEGORIZED);

        // TODO figure out a better way to handle the renderer init parameters. 500000 should not be hardcoded
        Renderer::Init(m_WindowDimensions.first, m_WindowDimensions.second);
        this->RegisterEventCallback(Renderer::OnEvent, EventCategory::UNCATEGORIZED);

        Physics::Init();
        this->RegisterEventCallback(Physics::OnEvent, EventCategory::UNCATEGORIZED);
    }

    void Application::Run() {

        if (m_AppRunning) {
            return;
        }

        this->Start();

        ApplicationStartEvent startEvent;
        OnEvent(startEvent);

        m_AppRunning = true;

        while (m_AppRunning) {

            // Calculate the time since last frame in seconds
            float deltaTime = GetTimeSinceLastChecked() / 1000000.0f;
            this->Update(deltaTime);

            // If the app was shut down in the update, break out of the loop
            if (!m_AppRunning) {
                break;
            }

            // Send an app update into the event system
            ApplicationUpdateEvent event(deltaTime);
            OnEvent(event);
        }
        this->Shutdown();
    }

    void Application::OnEvent(Event& event) {

        EventValidation::ValidateEvent(event);

        if (m_ShutdownSeen) {
            Global::fileLogger.Warning("Sprocket: Event received after shutdown event. Event will be ignored.");
            return;
        }

        // Traverse the callbacks in reverse order. Right now this is done so we can register the window
        // and renderer first. That way we can assure they receive events, mainly update, last
        for (int i = m_EventCallbacks.size() - 1; i >= 0; i--) {

            // If somewhere in the loop a shutdown event was happened, do not continue this loop
            if (m_ShutdownSeen) {
                break;
            }

            if (event.IsCategory(m_EventCallbacks[i].second)) {

                // Post the event to the subscriber
                m_EventCallbacks[i].first(event);

            }
        }

        if (event.GetEventType() == EventType::APP_SHUTDOWN) {
            Global::fileLogger.Info("Sprocket: Shutdown");
            m_AppRunning = false;
            m_ShutdownSeen = true;
        }
    }

    void Application::RegisterEventCallback(std::function<void(Event&)> eventCallback, EventCategory category) {
        m_EventCallbacks.push_back(std::pair(eventCallback, category));
    }

    int64_t Application::GetTimeSinceLastChecked() {
        auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto elapsed = currentMicro - m_LastTimeChecked;
        m_LastTimeChecked = currentMicro;
        return elapsed;
    }

}