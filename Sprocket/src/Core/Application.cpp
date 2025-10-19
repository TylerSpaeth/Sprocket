#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Events/EventValidation.h"
#include "Core/Global.h"

#include "Core/Window.h"
#include "Core/Input.h"
#include "Renderer/Renderer.h"
#include "Scene/SceneManager.h"
#include "ImGui/ImGuiImpl.h"
#include "Physics/PhysicsManager.h"
#include "Audio/AudioMananger.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Application::Init() {

        // It is acceptable to allocate systems and lose their reference since they will clean 
        // themselves up when the recieve a shutdown event

        if (m_Initialized) {
            return;
        }

        Global::FileLogger().Info("Sprocket: Startup");

        Window::Init(m_WindowDimensions.first, m_WindowDimensions.second, m_WindowTitle);
        Window::RegisterEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        this->RegisterEventCallback(Window::OnEvent, EventCategory::UNCATEGORIZED);

        Global::FileLogger().Info("Window Initialized.");

        Input::Init();
        this->RegisterEventCallback(Input::OnEvent, EventCategory::APPLICATION);

        Global::FileLogger().Info("Input Initialized.");

        // ImGui must be initialized after the window and much have its callback registered before the renderer
        ImGuiImpl::Init();
        this->RegisterEventCallback(ImGuiImpl::OnEvent, EventCategory::UNCATEGORIZED);

        Global::FileLogger().Info("ImGuiImpl Intialized.");
        
        auto renderer = new Renderer();
        this->RegisterEventCallback(std::bind(&Renderer::OnEvent, renderer, std::placeholders::_1), EventCategory::UNCATEGORIZED);

        Global::FileLogger().Info("Renderer Initialized.");

        auto physics = new PhysicsManager();
        this->RegisterEventCallback(std::bind(&PhysicsManager::OnEvent, physics, std::placeholders::_1), EventCategory::UNCATEGORIZED);

        Global::FileLogger().Info("Physics Initialized.");

        auto audioManager = new AudioManager();
        this->RegisterEventCallback(std::bind(&AudioManager::OnEvent, audioManager, std::placeholders::_1), EventCategory::UNCATEGORIZED);

        Global::FileLogger().Info("AudioManager Intialized.");

        SceneManager::Init(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        this->RegisterEventCallback(SceneManager::OnEvent, EventCategory::UNCATEGORIZED);

        Global::FileLogger().Info("SceneManager Initialized.");
    }

    void Application::Run() {

        if (m_AppRunning) {
            return;
        }

        ApplicationStartEvent* startEvent = new ApplicationStartEvent(m_WindowDimensions.first, m_WindowDimensions.second);
        OnEvent(*startEvent);
        delete startEvent;

        // We consider the app to be running once all of the systems have started. 
        m_AppRunning = true;
        
        this->Start();

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

        if (!m_AppRunning && event.GetEventType() != EventType::APP_START) {
            Global::FileLogger().Warning("Event recieved before application startup. Event will be ignored.");
            return;
        }

        if (m_ShutdownSeen) {
            Global::FileLogger().Warning("Event received after shutdown event. Event will be ignored.");
            return;
        }

        EventValidation::ValidateEvent(event);

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
            Global::FileLogger().Info("Sprocket: Shutdown");
            m_AppRunning = false;
            m_ShutdownSeen = true;
        }
    }

    void Application::RegisterEventCallback(std::function<void(Event&)> eventCallback, EventCategory category) {
        m_EventCallbacks.push_back(std::pair(eventCallback, category));
    }

    void Application::SetWindowTitle(const std::string& title) {
        m_WindowTitle = title;
    }

    void Application::SetWindowDimensions(unsigned int width, unsigned int height) {
        m_WindowDimensions = {width, height};
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////PROTECTED///////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Application::Start() {}
    void Application::Update(float deltaTime) {}
    void Application::Shutdown() {}

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    const int64_t Application::GetTimeSinceLastChecked() {
        auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto elapsed = currentMicro - m_LastTimeChecked;
        m_LastTimeChecked = currentMicro;
        return elapsed;
    }

}