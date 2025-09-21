#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "Scene.h"
#include "Core/Macros.h"
#include "Events/Event.h"

#include "Core/Sprocket.pch"

#include "Utils/Singleton.h"

namespace Sprocket {

    /// @brief This is a singleton that handles the storage and management of unique scenes 
    /// that may make up a given Sprocket application. This is an event subscriber.
    class SPROCKET_API SceneManager : public Singleton<SceneManager> {

    private:

        bool m_ChangeScene = false;
        int m_NewActiveScene = 0;

        // An empty default scene will always be given at index 0.
        int m_ActiveSceneIndex = 0;
        mutable std::unordered_map<int, const Scene*> m_Scenes;

        std::function<void(Event&)> m_EventCallback;

        /// @brief Checks if the scene should change, otherwise passes through the event.
        /// @param event The update event to pass through in the scene is not changing
        static void OnUpdate(Event& event);

    public:
        /// @brief Initializes the SceneManager singleton. This function
        /// must be called before any other SceneManager functions.
        /// @param eventCallback an event callback to be used by the scene manager and any connected 
        /// components
        static void Init(const std::function<void(Event&)> eventCallback);

        /// @brief Adds a new scene that can be retrieved at the given index.
        /// @param index the index that the scene should be stored at
        /// @param scene a pointer to a scene that should be stored 
        /// @returns true if adding is successful, false otherwise
        static bool AddScene(const int index, const Scene* scene);

        /// @brief Removes the scene at the given index.
        /// @param index the index of the scene that should be removed
        /// @return true if removal succeeds, false otherwise
        static bool RemoveScene(const int index);

        /// @brief Gets the scene at the given index.
        /// @param index the index of the scene that should be retrieved
        /// @return a pointer to the scene at the given index, nullptr if it does not exist.
        static Scene* GetSceneAtIndex(const int index);

        /// @brief Sets the active scene to that of the given index.
        /// @param index the index of the scene that should be made active.
        /// @return true if setting the active scene is successful, false otherwise
        static bool SetActiveScene(const int index);

        /// @brief Gets the active scene.
        /// @return a pointer to the active scene, nullptr if it does not exist
        static Scene* GetActiveScene();

        /// @brief Gets the index of the active scene.
        /// @return the index of the active scene.
        static int GetActiveSceneIndex() { return s_Instance->m_ActiveSceneIndex; }

        /// @brief Handles incoming events. Should be registered as a callback to receive events.
        /// @param event The event the should be handled.
        static void OnEvent(Event& event);  

    };

}

#endif