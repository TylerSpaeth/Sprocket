#ifndef APPLICATION_H
#define APPLICATION_H

#include "Macros.h"
#include "Events/Event.h"

#include <chrono>
#include <functional>
#include <vector>
#include <utility>

namespace Sprocket {

  /// @brief The central component of a Sprocket application. Drives the main run loop and 
  /// acts as the central event handler for the entire application. This is the central event
  /// handler for Sprocket, so most events should pass through here.
  class SPROCKET_API Application {
    private:
      bool m_AppRunning;
      int64_t m_LastTimeChecked = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
      int64_t GetTimeSinceLastChecked();
      std::vector<std::pair<std::function<void(Event&)>, EventCategory>> m_EventCallbacks;
    public:
      Application();
      virtual ~Application();
      
      /// @brief This function is the first thing run when the Run() function is called.
      /// For the time being, this is just a way for a user application to define behaviors
      /// at the start of a the run loop. In the future this will probably be removed.
      virtual void Start();// TODO remove when scripting is added

      /// @brief This function is called on every iteration of the Run loop. For the time being, 
      /// This is just a way for a user application to define behaviors that should be processed
      /// every frame. In the future this will probably be removed.
      /// @param deltaTime The amount of time that has elapsed since the last call to Update in seconds.
      virtual void Update(float deltaTime); // TODO remove when scripting is added

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
  };

  // Define in the code that is using Sprocket
  Application* CreateApplication();

}

#endif