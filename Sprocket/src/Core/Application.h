#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core/Sprocket.pch"

#include "Macros.h"
#include "Events/Event.h"

namespace Sprocket {

    /// @brief The central component of a Sprocket application. Drives the main run loop and 
    /// acts as the central event handler for the entire application. This is the central event
    /// handler for Sprocket, so most events should pass through here.
    class SPROCKET_API Application {
    private:

        bool m_Initialized = false;
        bool m_AppRunning = false;
        bool m_ShutdownSeen = false;

        int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

        std::vector<std::pair<std::function<void(Event&)>, EventCategory>> m_EventCallbacks;

        std::string m_WindowTitle = "Sprocket Application";
        std::pair<unsigned int, unsigned int> m_WindowDimensions = { 1066, 600 };

    public:

        virtual ~Application() = default;

        /// @brief Initializes the application. This function must be called before Run() is called.
        void Init();

        /// @brief Runs the program. Calls the Start() and Update() functions appropriately and 
        /// disperses ApplicationUpdateEvents to all subscribers in every iteration of the loop.
        void Run();

        /// @brief Takes in an event and creates an EventHandler to disperse the event to all
        /// subscribers. This function should be used as a callback by all Event producers in 
        /// order to send events into the central event system.
        /// @param event The event that needs to be handled.
        void OnEvent(Event& event);

        /// @brief Registers the given function as an Event callback to be run when an event is 
        /// recieved. Anything that wishes to subscribe directly to the central event system should 
        /// register a callback with this function.
        /// @param eventCallback a function that will take in an Event when an event occurs.
        /// @param category The category of events that should be sent to the callback. UNCATEGORIZED 
        /// will send all events.
        void RegisterEventCallback(std::function<void(Event&)> eventCallback, EventCategory category);

        /// @brief Sets the title of the application window.
        /// @param title The title to set the window to.
        void SetWindowTitle(const std::string& title);

        /// @brief Sets the dimensions of the application window.
        /// @param width The width of the window in pixels.
        /// @param height The height of the window in pixels.
        void SetWindowDimensions(unsigned int width, unsigned int height);

    protected:

        /// @brief This function is the first thing run when the Run() function is called.
        /// For the time being, this is just a way for a user application to define behaviors
        /// at the start of a the run loop. In the future this will probably be removed.
        virtual void Start();// TODO remove when scripting is added

        /// @brief This function is called on every iteration of the Run loop. For the time being, 
        /// This is just a way for a user application to define behaviors that should be processed
        /// every frame. In the future this will probably be removed.
        /// @param deltaTime The amount of time that has elapsed since the last call to Update in seconds.
        virtual void Update(float deltaTime); // TODO remove when scripting is added

        /// @brief This function is called once after the main loop has ended.
        virtual void Shutdown(); // TODO remove when scripting is added

    private:

        /// @brief Gets the amount of time that has elapsed since this function was last called in 
        /// microseconds.
        /// @return Time elapsed since last called.
        int64_t GetTimeSinceLastChecked();
    };

    // Define in the code that is using Sprocket
    Application* CreateApplication();

}

#endif