#include "SceneManager.h"

#include <stdexcept>

namespace Sprocket {

  void SceneManager::AddScene(const int index, const Scene* scene) {
    if(m_Scenes.find(index) != m_Scenes.cend()) {
      throw std::invalid_argument("Given index is already in use.");
    }
    m_Scenes.insert({index, scene});
  }

  void SceneManager::RemoveScene(const int index) {
    if(m_Scenes.find(index) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    m_Scenes.erase(index);
  }

  Scene* SceneManager::GetSceneAtIndex(const int index) const {
    if(m_Scenes.find(index) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    return (Scene*) m_Scenes.at(index);
  }

  void SceneManager::SetActiveScene(const int index) const {
    if(m_Scenes.find(index) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    m_ActiveSceneIndex = index;
  }

  Scene* SceneManager::GetActiveScene() const {
    if(m_Scenes.find(m_ActiveSceneIndex) == m_Scenes.cend()) {
      throw std::invalid_argument("No scene exists with this index.");
    }
    return (Scene*) m_Scenes.at(m_ActiveSceneIndex);
  }

  void SceneManager::OnUpdate(Event& event) {
    // Propogate the call to the active scene
    GetActiveScene()->OnEvent(event); 
    // TODO handle events
  }

} 