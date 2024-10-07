#include "SceneManager.h"

#include <stdexcept>

namespace Sprocket {

  SceneManager* SceneManager::s_Instance = nullptr;
  void SceneManager::Init() {
    if(!s_Instance) {
      s_Instance = new SceneManager();
      s_Instance->AddSceneInstance(0, new Scene());
    }
  }

  void SceneManager::AddSceneInstance(const int index, const Scene* scene) {
    if(m_Scenes.find(index) != m_Scenes.cend()) {
      throw std::invalid_argument("Given index is already in use.");
    }
    m_Scenes.insert({index, scene});
  }

  void SceneManager::RemoveSceneInstance(const int index) {
    if(m_Scenes.find(index) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    m_Scenes.erase(index);
  }

  Scene* SceneManager::GetSceneAtIndexInstance(const int index) const {
    if(m_Scenes.find(index) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    return (Scene*) m_Scenes.at(index);
  }

  void SceneManager::SetActiveSceneInstance(const int index) {
    if(m_Scenes.find(index) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    m_ActiveSceneIndex = index;
  }

  Scene* SceneManager::GetActiveSceneInstance() const {
    if(m_Scenes.find(m_ActiveSceneIndex) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    return (Scene*) m_Scenes.at(m_ActiveSceneIndex);
  }

  void SceneManager::OnEventInstance(Event& event) {
    // Propogate the call to the active scene
    GetActiveScene()->OnEvent(event); 
    // TODO handle events
  }

} 