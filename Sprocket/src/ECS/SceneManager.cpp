#include "SceneManager.h"

#include <stdexcept>

namespace Sprocket {

  SceneManager* SceneManager::s_Instance = nullptr;
  void SceneManager::Init() {
    if(!s_Instance) {
      s_Instance = new SceneManager();
      s_Instance->AddScene(0, new Scene());
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

    // Remove the callback from the old active scene so that it can no longer submit events.
    s_Instance->GetActiveScene()->m_EventCallback = nullptr;
    s_Instance->GetActiveScene()->m_Root->m_EventCallback = nullptr;

    s_Instance->m_ActiveSceneIndex = index;
    // Make sure the active scene has the callback to send events to the application
    s_Instance->GetActiveScene()->m_EventCallback = s_Instance->m_EventCallback;
    s_Instance->GetActiveScene()->m_Root->m_EventCallback = s_Instance->m_EventCallback;
  }

  Scene* SceneManager::GetActiveScene() {
    if(s_Instance->m_Scenes.find(s_Instance->m_ActiveSceneIndex) == s_Instance->m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    return (Scene*) s_Instance->m_Scenes.at(s_Instance->m_ActiveSceneIndex);
  }

  void SceneManager::OnEvent(Event& event) {
    // Propogate the call to the active scene
    GetActiveScene()->OnEvent(event); 
    // TODO handle events
  }

  void SceneManager::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
    s_Instance->m_EventCallback = eventCallback;
    // Make sure the active scene has the callback to send events to the application
    s_Instance->GetActiveScene()->m_EventCallback = eventCallback;
    s_Instance->GetActiveScene()->m_Root->m_EventCallback = eventCallback;
  }

} 