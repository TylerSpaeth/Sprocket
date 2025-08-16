#ifndef WINDOW_H
#define WINDOW_H

#include "Events/Event.h"
#include "Macros.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

  /// @brief This is a singleton the encapsulates the GLFW functionality. This is an event producer.
  class SPROCKET_API Window {
    private:
      void* m_Window; // GLFWwindow
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
	  void EnableCursorInstance();
	  void DisableCursorInstance();
	  void HideCursorInstance();

      /// @brief This function is called by the OnEvent function when a WINDOW_CLOSE event
      /// is recieved. Performs any tasks that need to occur before Input is destructed.
      static void OnClose() {s_Instance->OnCloseInstance();}
      /// @brief This function is called by the OnEvent function when a APP_UPDATE event is
      /// received. Performs any tasks that need to occur every repeatedly.
      static void OnUpdate() {s_Instance->OnUpdateInstance();}

    public:
      /// @brief Initializes the Window singleton and sets appropriate values. This function
      /// must be called before any other Window functions.
      /// @param xDimension The width of the window in pixels.
      /// @param yDimension The height of the window in pixels.
      /// @param windowTitle The text that should be on the title bar of the window.
      static void Init(const unsigned int xDimension, const unsigned int yDimension, const std::string& windowTitle);

      /// @brief Handles incoming events. Should be registered as a callback to receive events.
      /// @param event The event the should be handled.
      static void OnEvent(Event& event) {s_Instance->OnEventInstance(event);}

      /// @brief Registers the given function as an Event callback to be run when a window event 
      /// occurs. The event handler should subscribe to this in order for Window events to become
      /// part of the central event system.
      /// @param eventCallback a function that will take in an Event when an event occurs.
      static void RegisterEventCallback(const std::function<void(Event&)> eventCallback) {s_Instance->RegisterEventCallbackInstance(eventCallback);}

      /// @brief Enables or disables vsync on the window. Enabled by default.
      /// @param enable if true, then vsync will be enable. If false, it will be disabled.
      static void EnableVSync(bool enable) {s_Instance->EnableVSyncInstance(enable);}

      static void EnableCursor() {s_Instance->EnableCursorInstance();}

	  static void DisableCursor() {s_Instance->DisableCursorInstance();}

      static void HideCursor() {s_Instance->HideCursorInstance();}
  };

}

#endif