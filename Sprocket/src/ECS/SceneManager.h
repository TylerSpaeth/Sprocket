#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Core/Macros.h"
#include "Events/Event.h"

#include <unordered_map>

namespace Sprocket {

  /// @brief This is a singleton that handles the storage and management of unique scenes 
  /// that may make up a given Sprocket application. This is an event subscriber.
  class SPROCKET_API SceneManager {

    private:
      // An empty default scene will always be given at index 0.
      mutable int m_ActiveSceneIndex = 0;
      mutable std::unordered_map<int, const Scene*> m_Scenes;

      // Singleton Components
      static SceneManager* s_Instance;
      SceneManager(){}
      SceneManager(const SceneManager&) = delete;
      SceneManager operator=(const SceneManager*) = delete;

      void AddSceneInstance(const int index, const Scene* scene);
      void RemoveSceneInstance(const int index);
      Scene* GetSceneAtIndexInstance(const int index) const;
      void SetActiveSceneInstance(const int index);
      Scene* GetActiveSceneInstance() const;
      int GetActiveSceneIndexInstance() const {return m_ActiveSceneIndex;}
      void OnEventInstance(Event& event);

    public:
      /// @brief Initializes the SceneManager singleton. This function
      /// must be called before any other SceneManager functions.
      static void Init();

      /// @brief Adds a new scene that can be retrieved at the given index.
      /// @param index the index that the scene should be stored at
      /// @param scene a pointer to a scene that should be stored 
      /// @throws invalid_argument if the given index is already in use
      static void AddScene(const int index, const Scene* scene) {s_Instance->AddSceneInstance(index, scene);}

      /// @brief Removes the scene at the given index.
      /// @param index the index of the scene that should be removed
      /// @throws invalid_argument if there are no scenes at the given index
      static void RemoveScene(const int index) {s_Instance->RemoveSceneInstance(index);}

      /// @brief Gets the scene at the given index.
      /// @param index the index of the scene that should be retrieved
      /// @return a pointer to the scene at the given index
      /// @throws invalid_argument if there are no scenes at the given index
      static Scene* GetSceneAtIndex(const int index) {return s_Instance->GetSceneAtIndexInstance(index);}

      /// @brief Sets the active scene to that of the given index.
      /// @param index the index of the scene that should be made active.
      /// @throws invalid_argument if there are no scenes at the given index
      static void SetActiveScene(const int index) {s_Instance->SetActiveSceneInstance(index);}

      /// @brief Gets the active scene.
      /// @return a pointer to the active scene.
      /// @throws invalid_argument if there are no scene at the active index.
      static Scene* GetActiveScene() {return s_Instance->GetActiveSceneInstance();}

      /// @brief Gets the index of the active scene.
      /// @return the index of the active scene.
      /// @throws invalid_argument if there are no scene at the active index.
      static int GetActiveSceneIndex() {return s_Instance->GetActiveSceneIndexInstance();}

      /// @brief Handles incoming events. Should be registered as a callback to receive events.
      /// @param event The event the should be handled.
      static void OnEvent(Event& event) {s_Instance->OnEventInstance(event);}
  };

}

#endif