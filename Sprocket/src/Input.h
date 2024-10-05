#ifndef INPUT_H
#define INPUT_H

#include "Macros.h"
#include "Keycodes.h"
#include "MouseButtons.h"
#include "Events/Event.h"

#include <unordered_map>
#include <vector>

namespace Sprocket {
  
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

    public:
      static void Init();
      static void OnEvent(Event& event) {s_Instance->OnEventInstance(event);}
      static void OnClose() {s_Instance->OnCloseInstance();}
      static void OnUpdate() {s_Instance->OnUpdateInstance();}
      static bool IsKeyPressed(Keycode key) {return s_Instance->IsKeyPressedInstance(key);}
      static bool IsMouseButtonPressed(MouseButton button) {return s_Instance->IsMouseButtonPressedInstance(button);}
      static float GetMouseXPosition() {return s_Instance->m_MouseXPosition;}
      static float GetMouseYPosition() {return s_Instance->m_MouseYPosition;}
      static float GetMouseXScrollOffset() {return s_Instance->m_MouseXScrollOffset;}
      static float GetMouseYScrollOffset() {return s_Instance->m_MouseYScrollOffset;}

  };

}

#endif