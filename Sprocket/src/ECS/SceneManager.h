#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Core/Macros.h"

#include <unordered_map>

namespace Sprocket {

  /// @brief This class handles the storage and management of unique scenes that may make up
  /// a given Sprocket application.
  class SPROCKET_API SceneManager {
    private:
      mutable int m_ActiveSceneIndex = 0;
      mutable std::unordered_map<int, const Scene*> m_Scenes;
    public:
      SceneManager() = default;
      ~SceneManager() = default;

      /// @brief Adds a new scene that can be retrieved at the given index.
      /// @param index the index that the scene should be stored at
      /// @param scene a pointer to a scene that should be stored 
      /// @throws invalid_argument if the given index is already in use
      void AddScene(const int index, const Scene* scene);

      /// @brief Removes the scene at the given index.
      /// @param index the index of the scene that should be removed
      /// @throws invalid_argument if there are no scenes at the given index
      void RemoveScene(const int index);

      /// @brief Gets the scene at the given index.
      /// @param index the index of the scene that should be retrieved
      /// @return a pointer to the scene at the given index
      /// @throws invalid_argument if there are no scenes at the given index
      Scene* GetSceneAtIndex(const int index) const;

      /// @brief Sets the active scene to that of the given index.
      /// @param index the index of the scene that should be made active.
      /// @throws invalid_argument if there are no scenes at the given index
      void SetActiveScene(const int index) const;

      /// @brief Gets the active scene.
      /// @return a pointer to the active scene.
      /// @throws invalid_argument if there are no scene at the active index.
      Scene* GetActiveScene() const;

      /// @brief Gets the index of the active scene.
      /// @return the index of the active scene.
      /// @throws invalid_argument if there are no scene at the active index.
      int GetActiveSceneIndex() const {return m_ActiveSceneIndex;}
  };

}

#endif