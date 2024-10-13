#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Core/Macros.h"
#include "Events/Event.h"

#include <unordered_map>
#include <functional>

namespace Sprocket {

  /// @brief This is a singleton that handles the storage and management of unique scenes 
  /// that may make up a given Sprocket application. This is an event subscriber.
  class SPROCKET_API SceneManager {

    private:
      // An empty default scene will always be given at index 0.
      mutable int m_ActiveSceneIndex = 0;
      mutable std::unordered_map<int, const Scene*> m_Scenes;
      
      std::function<void(Event&)> m_EventCallback;

      // Singleton Components
      static SceneManager* s_Instance;
      SceneManager(){}
      SceneManager(const SceneManager&) = delete;
      SceneManager operator=(const SceneManager*) = delete;

    public:
      /// @brief Initializes the SceneManager singleton. This function
      /// must be called before any other SceneManager functions.
      static void Init();

      /// @brief Adds a new scene that can be retrieved at the given index.
      /// @param index the index that the scene should be stored at
      /// @param scene a pointer to a scene that should be stored 
      /// @throws invalid_argument if the given index is already in use
      static void AddScene(const int index, const Scene* scene);

      /// @brief Removes the scene at the given index.
      /// @param index the index of the scene that should be removed
      /// @throws invalid_argument if there are no scenes at the given index
      static void RemoveScene(const int index);

      /// @brief Gets the scene at the given index.
      /// @param index the index of the scene that should be retrieved
      /// @return a pointer to the scene at the given index
      /// @throws invalid_argument if there are no scenes at the given index
      static Scene* GetSceneAtIndex(const int index);

      /// @brief Sets the active scene to that of the given index.
      /// @param index the index of the scene that should be made active.
      /// @throws invalid_argument if there are no scenes at the given index
      static void SetActiveScene(const int index);

      /// @brief Gets the active scene.
      /// @return a pointer to the active scene.
      /// @throws invalid_argument if there are no scene at the active index.
      static Scene* GetActiveScene();

      /// @brief Gets the index of the active scene.
      /// @return the index of the active scene.
      /// @throws invalid_argument if there are no scene at the active index.
      static int GetActiveSceneIndex() {return s_Instance->m_ActiveSceneIndex;}

      /// @brief Handles incoming events. Should be registered as a callback to receive events.
      /// @param event The event the should be handled.
      static void OnEvent(Event& event);

      /// @brief Registers the given function as an Event callback to be run when an event occurs 
      /// in the ECS system.. The event handler should subscribe to this in order for events 
      /// produced here to become part of the central event system.
      /// @param eventCallback a function that will take in an Event when an event occurs.
      static void RegisterEventCallback(const std::function<void(Event&)> eventCallback);
  };

}

#endif