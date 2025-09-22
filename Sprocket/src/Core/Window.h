#ifndef WINDOW_H
#define WINDOW_H

#include "Events/Event.h"
#include "Macros.h"

#include "Core/Sprocket.pch"

#include "Utils/Singleton.h"

namespace Sprocket {

    /// @brief This is a singleton the encapsulates the GLFW functionality. This is an event producer.
    class SPROCKET_API Window : public Singleton<Window> {
        friend class Application;
    private:

        void* m_Window; // GLFWwindow
        std::function<void(Event&)> m_EventCallback;

        int m_XDimension = 0;
        int m_YDimension = 0;

    public:

        /// @brief Enables or disables vsync on the window. Enabled by default.
        /// @param enable if true, then vsync will be enable. If false, it will be disabled.
        static void EnableVSync(bool enable);

        /// @brief Enables the cursor to be visibile. This is the default
        static void EnableCursor();

        /// @brief Disables the cursor.
        static void DisableCursor();

        /// @brief Hides the cursor but does not disable it.
        static void HideCursor();

        /// @brief Locks the aspect ratio to what it is currently set to.
        static void LockAspectRatio();

        /// @brief Unlocks the aspect ratio of the window.
        static void UnlockAspectRatio();

    private:

        /// @brief Initializes the Window singleton and sets appropriate values. This function
        /// must be called before any other Window functions.
        /// @param xDimension The width of the window in pixels.
        /// @param yDimension The height of the window in pixels.
        /// @param windowTitle The text that should be on the title bar of the window.
        static void Init(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle);

        /// @brief Handles incoming events. Should be registered as a callback to receive events.
        /// @param event The event the should be handled.
        static void OnEvent(Event& event);

        /// @brief Registers the given function as an Event callback to be run when a window event 
        /// occurs. The event handler should subscribe to this in order for Window events to become
        /// part of the central event system.
        /// @param eventCallback a function that will take in an Event when an event occurs.
        static void RegisterEventCallback(const std::function<void(Event&)> eventCallback);

        /// @brief This function is called by the OnEvent function when a APP_SHUTDOWN event
        /// is recieved. Performs any tasks that need to occur before Input is destructed.
        static void OnShutdown();
        /// @brief This function is called by the OnEvent function when a APP_UPDATE event is
        /// received. Performs any tasks that need to occur every repeatedly.
        static void OnUpdate();

    };

}

#endif