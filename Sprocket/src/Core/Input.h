#ifndef INPUT_H
#define INPUT_H

#include "Core/Sprocket.pch"

#include "Macros.h"
#include "Keycodes.h"
#include "MouseButtons.h"
#include "Events/Event.h"

#include "Utils/Singleton.h"

namespace Sprocket {


    /// @brief Input is a singleton that stores input event information that it recieves throught its
    /// OnEvent function. This is an event subscriber.
    class SPROCKET_API Input : public Singleton<Input> {
    private:

        float m_MouseXPosition;
        float m_MouseYPosition;
        float m_MouseXScrollOffset;
        float m_MouseYScrollOffset;

        mutable std::unordered_map<Keycode, bool> m_CurrentKeyStatus;
        mutable std::unordered_map<MouseButton, bool> m_CurrentButtonStatus;

    public:
        /// @brief Initializes the singleton. This function must be called before any other Input 
        /// functions to insure that it works properly.
        static void Init();

        /// @brief Handles incoming events. Should be registered as a callback to receive events.
        /// @param event The event the should be handled.
        static void OnEvent(Event& event);

        /// @brief Checks if the given key is pressed.
        /// @param key The keycode corresponding to the key that should be checked.
        /// @return true if the key is pressed, otherwise false.
        static bool IsKeyPressed(Keycode key);

        /// @brief Checks if the given mouse button is pressed.
        /// @param button The mouse button code corresponding to the key that should be checked.
        /// @return true if the button is pressed, otherwise false.
        static bool IsMouseButtonPressed(MouseButton button);

        static float GetMouseXPosition();
        static float GetMouseYPosition();
        static float GetMouseXScrollOffset();
        static float GetMouseYScrollOffset();

    private:

        /// @brief This function is called by the OnEvent function when a APP_UPDATE event is
        /// received. Performs any tasks that need to occur every repeatedly.
        static void OnUpdate();

        // Actual implementations for the static instance functions
        /// @brief This function is called by the OnEvent function when a APP_SHUTDOWN event
        /// is recieved. Performs any tasks that need to occur before Input is destructed.
        static void OnShutdown();

    };

}

#endif