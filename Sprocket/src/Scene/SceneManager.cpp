#include "SceneManager.h"

#include <stdexcept>
#include <iostream>

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

  void SceneManager::AddScene(const int index, const Scene* scene) {
    if(s_Instance->m_Scenes.find(index) != s_Instance->m_Scenes.cend()) {
      throw std::invalid_argument("Given index is already in use.");
    }
    s_Instance->m_Scenes.insert({index, scene});
  }

  void SceneManager::RemoveScene(const int index) {
    if(s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    if(s_Instance->m_ActiveSceneIndex == index) {
      throw std::invalid_argument("The active scene can not be removed.");
    }
    s_Instance->m_Scenes.erase(index);
  }

  Scene* SceneManager::GetSceneAtIndex(const int index) {
    if(s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    return (Scene*) s_Instance->m_Scenes.at(index);
  }

  void SceneManager::SetActiveScene(const int index) {
    if(s_Instance->m_Scenes.find(index) == s_Instance->m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }

    // Remove the event callback of the current scene
    GetActiveScene()->RegisterEventCallback(nullptr);
    s_Instance->m_ActiveSceneIndex = index;
    // Set the event callback of the new scene
    GetActiveScene()->RegisterEventCallback(s_Instance->m_EventCallback);
    
  }

  Scene* SceneManager::GetActiveScene() {
    if(s_Instance->m_Scenes.find(s_Instance->m_ActiveSceneIndex) == s_Instance->m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    return (Scene*) s_Instance->m_Scenes.at(s_Instance->m_ActiveSceneIndex);
  }

}