#ifndef WINDOW_H
#define WINDOW_H

#include "Events/Event.h"
#include "Macros.h"

#include "ThirdParty/glad/glad.h"
#include "ThirdParty/GLFW/glfw3.h"

#include <string>
#include <functional>

namespace Sprocket {

  class SPROCKET_API Window {
    private:
      GLFWwindow* m_Window;
      std::function<void(Event&)> m_EventCallback;

      // Singleton components
      static Window* s_Instance;
      Window(){}
      Window(const Window&) = delete;
      Window operator=(const Window&) = delete;

      // Actual implementations for the static instance functions
      void OnEventInstance(Event& event);
      void OnCloseInstance();
      void OnUpdateInstance();
      void RegisterEventCallbackInstance(const std::function<void(Event&)> eventCallback);
      void EnableVSyncInstance(bool enable);

    public:
      static void Init(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle);
      static void OnEvent(Event& event) {s_Instance->OnEventInstance(event);}
      static void OnClose() {s_Instance->OnCloseInstance();}
      static void OnUpdate() {s_Instance->OnUpdateInstance();}
      static void RegisterEventCallback(const std::function<void(Event&)> eventCallback) {s_Instance->RegisterEventCallbackInstance(eventCallback);}
      static void EnableVSync(bool enable) {s_Instance->EnableVSyncInstance(enable);}
  };

}

#endif