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
#include "Audio/AudioMananger.h"

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

        Window::Init(m_WindowDimensions.first, m_WindowDimensions.second, m_WindowTitle);
        Window::RegisterEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        this->RegisterEventCallback(Window::OnEvent, EventCategory::UNCATEGORIZED);

        Global::fileLogger.Info("Window Initialized.");

        Input::Init();
        this->RegisterEventCallback(Input::OnEvent, EventCategory::APPLICATION);

        Global::fileLogger.Info("Input Initialized.");

        // ImGui must be initialized after the window and much have its callback registered before the renderer
        ImGuiImpl::Init();
        this->RegisterEventCallback(ImGuiImpl::OnEvent, EventCategory::UNCATEGORIZED);

        Global::fileLogger.Info("ImGuiImpl Intialized.");
        
        // It is acceptable to allocate this an lose reference to it because the shutdown event is what
        // should free it.
        auto renderer = new Renderer();
        this->RegisterEventCallback(std::bind(&Renderer::OnEvent, renderer, std::placeholders::_1), EventCategory::UNCATEGORIZED);

        Global::fileLogger.Info("Renderer Initialized.");

        Physics::Init();
        this->RegisterEventCallback(Physics::OnEvent, EventCategory::UNCATEGORIZED);

        Global::fileLogger.Info("Physics Initialized.");

        AudioManager::Init();
        this->RegisterEventCallback(AudioManager::OnEvent, EventCategory::AUDIO);

        Global::fileLogger.Info("AudioManager Intialized.");

        SceneManager::Init(std::bind(&Application::OnEvent, this, std::placeholders::_1));
        this->RegisterEventCallback(SceneManager::OnEvent, EventCategory::UNCATEGORIZED);

        Global::fileLogger.Info("SceneManager Initialized.");
    }

    void Application::Run() {

        if (m_AppRunning) {
            return;
        }

        ApplicationStartEvent* startEvent = new ApplicationStartEvent(m_WindowDimensions.first, m_WindowDimensions.second);
        OnEvent(*startEvent);
        delete startEvent;
        
        this->Start();

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