#include "ECS/Scene.h"
#include "ECS/QuadRenderer.h"

namespace Sprocket {

  void Scene::OnEvent(Event& event) {
    //TODO handle events
  }

  unsigned int Scene::CreateEntity() {
    m_Transforms.insert({m_EntityCount, TransformComponent()});
    m_Children.insert({m_EntityCount, std::vector<unsigned int>()});
    return m_EntityCount++;
  }

  // TODO there is a better way to do this, this is just going to work for now
  void Scene::SetEntityParent(const unsigned int entityID, const unsigned int parentID) {
    (*m_Children.find(parentID)).second.push_back(entityID);
  }

  void Scene::AddComponent(const unsigned int entityID, const Component& component) {
    switch(component.componentType) {
      case ComponentType::TRANSFORM_COMPONENT:
        // TODO should not be able to add a transform because they are there by default
        break;
      case ComponentType::QUAD_RENDERER_COMPONENT:
        try {
          m_QuadRenderers.at(entityID);
        } 
        catch(const std::exception& e) {
          m_QuadRenderers.insert({entityID,(QuadRendererComponent&)component});
        }
        QuadRenderer::RenderNewQuad(m_Transforms.at(entityID), m_QuadRenderers.at(entityID));
        QuadRenderer::UpdateQuad(m_Transforms.at(entityID), m_QuadRenderers.at(entityID));
        QuadRenderer::SetModelMatrix(m_Transforms.at(entityID), m_QuadRenderers.at(entityID));
        break;
    }
  }

  void Scene::UpdateComponent(const unsigned int entityID, const Component& replacement) {
    switch(replacement.componentType) {
      case ComponentType::TRANSFORM_COMPONENT:
        // Try to remove the old transform if there is one and replace it with the new transform, 
        // then update the model matrix in the quad renderer
        try {
          m_Transforms.extract(entityID);
        } 
        catch(const std::exception& e) {}
        m_Transforms.insert({entityID,(TransformComponent&)replacement});
        QuadRenderer::SetModelMatrix(m_Transforms.at(entityID), m_QuadRenderers.at(entityID));

        // FIXME This is not working properly due to the fact that everything is being stored as 
        // global position, may want to create seperate transform arrays with global and local 
        // positions
        for(unsigned int i : m_Children.at(entityID)) {
          TransformComponent t = m_Transforms.at(i);
          t.position += m_Transforms.at(entityID).position;
          t.rotation += m_Transforms.at(entityID).rotation;
          t.scale *= m_Transforms.at(entityID).scale;
          UpdateComponent(i,t);
        }

        break;
      case ComponentType::QUAD_RENDERER_COMPONENT:
        // Try to remove the old quad renderer and replace it with the new one. Then send an update 
        // quad call to the quad renderer so the updated data is reflected.
        try {
          m_QuadRenderers.extract(entityID);
        } 
        catch(const std::exception& e) {}
        m_QuadRenderers.insert({entityID,(QuadRendererComponent&)replacement});
        QuadRenderer::UpdateQuad(m_Transforms.at(entityID), m_QuadRenderers.at(entityID));
        break;
    }
  }

  TransformComponent Scene::GetTransform(const unsigned int entityID) {
    return m_Transforms.at(entityID);
  }

  QuadRendererComponent Scene::GetQuadRenderer(const unsigned int entityID) {
    return m_QuadRenderers.at(entityID);
  }
  
  

}