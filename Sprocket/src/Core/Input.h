#ifndef INPUT_H
#define INPUT_H

#include "Macros.h"
#include "Keycodes.h"
#include "MouseButtons.h"
#include "Events/Event.h"

#include <unordered_map>
#include <vector>

namespace Sprocket {
  

  /// @brief Input is a singleton that stores input event information that it recieves throught its
  /// OnEvent function. This is an event subscriber.
  class SPROCKET_API Input {
    private:

      float m_MouseXPosition;
      float m_MouseYPosition;
      float m_MouseXScrollOffset;
      float m_MouseYScrollOffset;

      mutable std::unordered_map<Keycode, bool> m_CurrentKeyStatus; 
      mutable std::unordered_map<MouseButton, bool> m_CurrentButtonStatus; 

      // Singleton components
      static Input* s_Instance;
      Input() : m_MouseXPosition(0), m_MouseYPosition(0), m_MouseXScrollOffset(0), m_MouseYScrollOffset(0) {}
      Input(const Input&) = delete;
      Input operator=(const Input) = delete;

      // Actual implementations for the static instance functions

      void OnEventInstance(Event& event);
      void OnCloseInstance();
      void OnUpdateInstance();
      bool IsKeyPressedInstance(Keycode key);
      bool IsMouseButtonPressedInstance(MouseButton button);

      /// @brief This function is called by the OnEvent function when a WINDOW_CLOSE event
      /// is recieved. Performs any tasks that need to occur before Input is destructed.
      static void OnClose() {s_Instance->OnCloseInstance();}
      /// @brief This function is called by the OnEvent function when a APP_UPDATE event is
      /// received. Performs any tasks that need to occur every repeatedly.
      static void OnUpdate() {s_Instance->OnUpdateInstance();}

    public:
      /// @brief Initializes the singleton. This function must be called before any other Input 
      /// functions to insure that it works properly.
      static void Init();

      /// @brief Handles incoming events. Should be registered as a callback to receive events.
      /// @param event The event the should be handled.
      static void OnEvent(Event& event) {s_Instance->OnEventInstance(event);}

      /// @brief Checks if the given key is pressed.
      /// @param key The keycode corresponding to the key that should be checked.
      /// @return true if the key is pressed, otherwise false.
      static bool IsKeyPressed(Keycode key) {return s_Instance->IsKeyPressedInstance(key);}

      /// @brief Checks if the given mouse button is pressed.
      /// @param button The mouse button code corresponding to the key that should be checked.
      /// @return true if the button is pressed, otherwise false.
      static bool IsMouseButtonPressed(MouseButton button) {return s_Instance->IsMouseButtonPressedInstance(button);}

      static float GetMouseXPosition() {return s_Instance->m_MouseXPosition;}
      static float GetMouseYPosition() {return s_Instance->m_MouseYPosition;}
      static float GetMouseXScrollOffset() {return s_Instance->m_MouseXScrollOffset;}
      static float GetMouseYScrollOffset() {return s_Instance->m_MouseYScrollOffset;}

  };

}

#endif