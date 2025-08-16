#include "SceneManager.h"

namespace Sprocket {

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////// EVENT HANDLING /////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

   void SceneManager::OnEvent(Event& event) {
    // Propogate the call to the active scene
    GetActiveScene()->OnEvent(event); 
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  SceneManager* SceneManager::s_Instance = nullptr;
  void SceneManager::Init(const std::function<void(Event&)> eventCallback) {
    if(!s_Instance) {
      s_Instance = new SceneManager();
      // Create a default scene with an index of 0
      s_Instance->AddScene(0, new Scene());

      // Store the event callback and register it with the newly created scene. Also load the scene
      // so that any of its subsystems can be activated.
      s_Instance->m_EventCallback = eventCallback;
      GetActiveScene()->RegisterEventCallback(eventCallback);
    }
  }

  bool SceneManager::AddScene(const int index, const Scene* scene) {
    if(s_Instance->m_Scenes.find(index) != s_Instance->m_Scenes.cend()) {
      return false;
    }
    s_Instance->m_Scenes.insert({index, scene});
    return true;
  }

  bool SceneManager::RemoveScene(const int index) {
    if(s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
      return false;
    }
    if(s_Instance->m_ActiveSceneIndex == index) {
      return false;
    }
    s_Instance->m_Scenes.erase(index);
    return true;
  }

  Scene* SceneManager::GetSceneAtIndex(const int index) {
    if(s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
      return nullptr;
    }
    return (Scene*) s_Instance->m_Scenes.at(index);
  }

  bool SceneManager::SetActiveScene(const int index) {
    if(s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
      return false;
    }

    // Remove the old scene
    GetActiveScene()->OnDeactivate();
    GetActiveScene()->RegisterEventCallback(nullptr);
    s_Instance->m_ActiveSceneIndex = index;

    // Setup the new scene
    GetActiveScene()->RegisterEventCallback(s_Instance->m_EventCallback);
    GetActiveScene()->OnActivate();

    return true;
    
  }

  Scene* SceneManager::GetActiveScene() {
    if(s_Instance->m_Scenes.find(s_Instance->m_ActiveSceneIndex) == s_Instance->m_Scenes.cend()) {
      return nullptr;
    }
    return (Scene*) s_Instance->m_Scenes.at(s_Instance->m_ActiveSceneIndex);
  }

}