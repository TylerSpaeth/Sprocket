#include "SceneManager.h"
#include "ECS/QuadRenderer.h"

#include <stdexcept>
#include <stack>

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

    switch(event.GetEventType()) {
      case EventType::APP_UPDATE:
        CheckModifiedComponents();
        break;
    }

  }

  void SceneManager::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
    s_Instance->m_EventCallback = eventCallback;
    // TODO register the eventcallback for all ECS systems
    QuadRenderer::s_Instance->m_EventCallback = eventCallback;
  }

  // TODO make sure this is well optimized because it is a limiting factor of the framerate
  void SceneManager::CheckModifiedComponents() {
    RootEntity* root = s_Instance->GetActiveScene()->GetSceneRoot();

    std::stack<Entity*> entities;

    for(Entity* e : root->GetChildren()) {
      entities.push(e);
    }

    while(!entities.empty()) {
      Entity* e = entities.top();
      entities.pop();

      // Add all children of current entity to the stack and if the transform of this
      // entity is modified, set the transform of the children as modified too
      for(Entity* ent : e->GetChildren()) {
        entities.push(ent);
        if(e->GetLocalTransform().modified) {
          ent->GetLocalTransform().modified = true;
        }
      } 

      // TODO go through here and check every component to see if it is modified and update the 
        // relevant systems with the updated data. If the transform of e is modified, then update 
        // all systems that use the transform
      for(Component* c : e->GetComponents()) {
        
        // If the component is uninitialized
        if(c->initialized == false) {
          if(c->componentType == ComponentType::QUAD_RENDERER) {
            QuadRenderer::RenderNewQuad(e->GetGlobalTransform(), *(QuadRendererComponent*)c);
            c->initialized = true;
          }
        }
        
        // If the transform is modified
        if(e->GetLocalTransform().modified) {
          if(c->componentType == ComponentType::QUAD_RENDERER) {
            QuadRenderer::SetModelMatrix(e->GetGlobalTransform(), *(QuadRendererComponent*)c);
            if(c->modified) {
              QuadRenderer::UpdateQuad(e->GetGlobalTransform(), *(QuadRendererComponent*)c);
              c->modified = false;
            }
          }
        }
        // If the transform is not modified
        else {
          if(c->componentType == ComponentType::QUAD_RENDERER && c->modified) {
             QuadRenderer::UpdateQuad(e->GetGlobalTransform(), *(QuadRendererComponent*)c);
              c->modified = false;
          }
        }
        
      }

      e->GetLocalTransform().modified = false;
      
    }


  }

} 