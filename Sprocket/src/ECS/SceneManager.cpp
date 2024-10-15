#include "SceneManager.h"
#include "ECS/QuadRenderer.h"

#include <stdexcept>
#include <queue>

namespace Sprocket {

  SceneManager* SceneManager::s_Instance = nullptr;
  QuadRenderer* QuadRenderer::s_Instance = nullptr;
  void SceneManager::Init() {
    if(!s_Instance) {
      s_Instance = new SceneManager();
      s_Instance->AddScene(0, new Scene());
      // TODO construct all ECS singletons here
      QuadRenderer::Init();
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

    s_Instance->m_ActiveSceneIndex = index;
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
    // TODO register the eventcallback for all ECS systems
    QuadRenderer::s_Instance->m_EventCallback = eventCallback;
  }

} 