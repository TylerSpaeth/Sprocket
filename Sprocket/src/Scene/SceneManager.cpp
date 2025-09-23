#include "SceneManager.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void SceneManager::OnEvent(Event& event) {
        switch (event.GetEventType()) {
            case EventType::APP_UPDATE:
                OnUpdate(event);
                break;
            default:
                // Propogate the call to the active scene
                GetActiveScene()->OnEvent(event);
                break;
        }
    }

    void SceneManager::Init(const std::function<void(Event&)> eventCallback) {
        if (!s_Instance) {
            s_Instance = new SceneManager();
            // Create a default scene with an index of 0
            s_Instance->AddScene(0, new Scene());

            // Store the event callback and register it with the newly created scene. Also load the scene
            // so that any of its subsystems can be activated.
            s_Instance->m_EventCallback = eventCallback;
            GetActiveScene()->RegisterEventCallback(eventCallback);
        }
    }

    const bool SceneManager::AddScene(const int index, const Scene* scene) {
        if (s_Instance->m_Scenes.find(index) != s_Instance->m_Scenes.cend()) {
            return false;
        }
        s_Instance->m_Scenes.insert({ index, scene });
        return true;
    }

    const bool SceneManager::RemoveScene(const int index) {
        if (s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
            return false;
        }
        if (s_Instance->m_ActiveSceneIndex == index) {
            return false;
        }
        s_Instance->m_Scenes.erase(index);
        return true;
    }

    Scene* SceneManager::GetSceneAtIndex(const int index) {
        if (s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
            return nullptr;
        }
        return (Scene*)s_Instance->m_Scenes.at(index);
    }

    const bool SceneManager::SetActiveScene(const int index) {
        if (s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
            return false;
        }

        s_Instance->m_NewActiveScene = index;
        s_Instance->m_ChangeScene = true;

        return true;

    }

    Scene* SceneManager::GetActiveScene() {
        if (s_Instance->m_Scenes.find(s_Instance->m_ActiveSceneIndex) == s_Instance->m_Scenes.cend()) {
            return nullptr;
        }
        return (Scene*)s_Instance->m_Scenes.at(s_Instance->m_ActiveSceneIndex);
    }

    const int SceneManager::GetActiveSceneIndex() {
        return s_Instance->m_ActiveSceneIndex;
    }

    const std::vector<int> SceneManager::GetAllSceneIndicies() {
        std::vector<int> keys;
        for (const auto& pair : s_Instance->m_Scenes) {
            keys.push_back(pair.first);
        }
        return keys;
    }

    const std::vector<const Scene*> SceneManager::GetAllScenes() {
        std::vector<const Scene*> scenes;
        for (const auto& pair : s_Instance->m_Scenes) {
            scenes.push_back(pair.second);
        }
        return scenes;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void SceneManager::OnUpdate(Event& event) {
        if (s_Instance->m_ChangeScene) {
            // Remove the old scene
            GetActiveScene()->OnDeactivate();
            GetActiveScene()->RegisterEventCallback(nullptr);
            s_Instance->m_ActiveSceneIndex = s_Instance->m_NewActiveScene;

            // Setup the new scene
            GetActiveScene()->RegisterEventCallback(s_Instance->m_EventCallback);
            GetActiveScene()->OnActivate();

            s_Instance->m_ChangeScene = false;
        }
        else {
            GetActiveScene()->OnEvent(event);
        }

    }

}