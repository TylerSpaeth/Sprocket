#include "ECS/Scene.h"
#include "ECS/QuadRenderer.h"
#include <iostream>
#include <algorithm>

namespace Sprocket {

  void Scene::OnEvent(Event& event) {
    //TODO handle events
  }

  unsigned int Scene::CreateEntity() {
    m_Transforms.push_back(TransformComponent());
    m_GlobalTransforms.push_back(TransformComponent());
    m_Children.push_back(std::vector<unsigned int>());
    m_Parents.push_back(-1);
    return m_EntityCount++;
  }

  // TODO there is a better way to do this, this is just going to work for now
  void Scene::SetEntityParent(const unsigned int entityID, const unsigned int parentID) {

    int currentParent = m_Parents.at(entityID);

    // If this entity has a parent, remove it from the parents children list and change the parentID
    if(currentParent != -1) {
      auto it = std::find(m_Children.at(currentParent).begin(), m_Children.at(currentParent).end(), entityID);
      m_Children.at(currentParent).erase(it);
      m_Parents.at(entityID) = parentID;
    }
   
    // Add this entity as a child of the given parent
    m_Children.at(parentID).push_back(entityID);

    // Calculate the global position of the parent and set that as the global position of the child
    TransformComponent global = m_GlobalTransforms.at(parentID);
    global.position += m_Transforms.at(parentID).position;
    global.rotation += m_Transforms.at(parentID).rotation;
    global.scale *= m_Transforms.at(parentID).scale;
    
    m_GlobalTransforms.at(entityID).position = global.position;
    m_GlobalTransforms.at(entityID).rotation = global.rotation;
    m_GlobalTransforms.at(entityID).scale = global.scale;

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
        QuadRenderer::UpdateQuad(m_QuadRenderers.at(entityID));

        TransformComponent globalTransform = m_GlobalTransforms.at(entityID);
        TransformComponent localTransform = m_Transforms.at(entityID);
        globalTransform.position += localTransform.position;
        globalTransform.rotation += localTransform.rotation;
        globalTransform.scale *= localTransform.scale;

        QuadRenderer::SetModelMatrix(globalTransform, m_QuadRenderers.at(entityID));
        break;
    }
  }

  void Scene::UpdateComponent(const unsigned int entityID, const Component& replacement) {
    switch(replacement.componentType) {
      case ComponentType::TRANSFORM_COMPONENT: {
        // Try to remove the old transform if there is one and replace it with the new transform, 
        // then update the model matrix in the quad renderer
        try {
          m_Transforms.at(entityID).position = ((TransformComponent&)replacement).position;
          m_Transforms.at(entityID).rotation = ((TransformComponent&)replacement).rotation;
          m_Transforms.at(entityID).scale = ((TransformComponent&)replacement).scale;
        } 
        catch(const std::exception& e) {}

        TransformComponent globalTransform = m_GlobalTransforms.at(entityID);
        TransformComponent localTransform = m_Transforms.at(entityID);
        globalTransform.position += localTransform.position;
        globalTransform.rotation += localTransform.rotation;
        globalTransform.scale *= localTransform.scale;

        QuadRenderer::SetModelMatrix(globalTransform, m_QuadRenderers.at(entityID));

        for(unsigned int i : m_Children.at(entityID)) {
          TransformComponent global = m_GlobalTransforms.at(entityID);
          TransformComponent localTransform = m_Transforms.at(entityID);
          global.position += localTransform.position;
          global.rotation += localTransform.rotation;
          global.scale *= localTransform.scale;

          m_GlobalTransforms.at(i).position = global.position;
          m_GlobalTransforms.at(i).rotation = global.rotation;
          m_GlobalTransforms.at(i).scale = global.scale;

          UpdateComponent(i,m_Transforms.at(i));
        }

        break;
      }
      case ComponentType::QUAD_RENDERER_COMPONENT: {
        // Try to remove the old quad renderer and replace it with the new one. Then send an update 
        // quad call to the quad renderer so the updated data is reflected.
        try {
          m_QuadRenderers.extract(entityID);
        } 
        catch(const std::exception& e) {}
        m_QuadRenderers.insert({entityID,(QuadRendererComponent&)replacement});
        QuadRenderer::UpdateQuad(m_QuadRenderers.at(entityID));
        break;
      }
    }
  }

  TransformComponent Scene::GetTransform(const unsigned int entityID) {
    return m_Transforms.at(entityID);
  }

  QuadRendererComponent Scene::GetQuadRenderer(const unsigned int entityID) {
    return m_QuadRenderers.at(entityID);
  }
  
  

}