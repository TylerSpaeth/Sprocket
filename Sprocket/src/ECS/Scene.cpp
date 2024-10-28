#include "ECS/Scene.h"
#include "ECS/QuadRenderer.h"
#include "ECS/Camera.h"

#include <iostream>
#include <algorithm>
#include <stdexcept>

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

    // If this entity has a parent, remove it from the parents children list
    if(currentParent != -1) {
      auto it = std::find(m_Children.at(currentParent).begin(), m_Children.at(currentParent).end(), entityID);
      m_Children.at(currentParent).erase(it);
    }
   
    // Add this entity as a child of the given parent
    m_Children.at(parentID).push_back(entityID);

    // Set the parentID
    m_Parents.at(entityID) = parentID;

    // Calculate the global position of the parent and set that as the global position of the child
    TransformComponent global = m_GlobalTransforms.at(parentID);
    global.position += m_Transforms.at(parentID).position;
    global.rotation += m_Transforms.at(parentID).rotation;
    global.scale *= m_Transforms.at(parentID).scale;
    
    m_GlobalTransforms.at(entityID).position = global.position;
    m_GlobalTransforms.at(entityID).rotation = global.rotation;
    m_GlobalTransforms.at(entityID).scale = global.scale;

  }

  void Scene::AddComponent(const unsigned int entityID, const QuadRendererComponent& component) {
    try {
      m_QuadRenderers.at(entityID);
    } 
    catch(const std::exception& e) {
      m_QuadRenderers.insert({entityID,component});
    }

    QuadRenderer::RenderNewQuad(m_Transforms.at(entityID), m_QuadRenderers.at(entityID));
    QuadRenderer::UpdateQuad(m_QuadRenderers.at(entityID));

    TransformComponent globalTransform = m_GlobalTransforms.at(entityID);
    TransformComponent localTransform = m_Transforms.at(entityID);
    globalTransform.position += localTransform.position;
    globalTransform.rotation += localTransform.rotation;
    globalTransform.scale *= localTransform.scale;

    QuadRenderer::SetModelMatrix(globalTransform, m_QuadRenderers.at(entityID));
  }

  void Scene::AddComponent(const unsigned int entityID, const CameraComponent& component) {

    if(m_CameraEntityID != -1) {
      std::runtime_error("There is already a camera in this scene.");
    }

    m_CameraEntityID = entityID;

    // TODO implement actual storage of a CameraComponent once the CameraComponent has actual 
    // data associated with it

  }

  void Scene::AddComponent(const unsigned int entityID, const BoxColliderComponent& component) {
    try {
      m_BoxColliders.at(entityID);
    }
    catch(const std::exception& e) {
      m_BoxColliders.insert({entityID,component});
    }
  }

  void Scene::AddComponent(const unsigned int entityID, const CircleColliderComponent& component) {
    try {
      m_CircleColliders.at(entityID);
    }
    catch(const std::exception& e) {
      m_CircleColliders.insert({entityID,component});
    }
  }

  void Scene::UpdateComponent(const unsigned int entityID, const TransformComponent& replacement) {
    // Update the local transform
    m_Transforms.at(entityID).position = replacement.position;
    m_Transforms.at(entityID).rotation = replacement.rotation;
    m_Transforms.at(entityID).scale = replacement.scale;

    // Calculate the global transform
    TransformComponent globalTransform = m_GlobalTransforms.at(entityID);
    TransformComponent localTransform = m_Transforms.at(entityID);
    globalTransform.position += localTransform.position;
    globalTransform.rotation += localTransform.rotation;
    globalTransform.scale *= localTransform.scale;

    // TODO also check that the entity has a QuadRendererComponent
    // Check whether this transform change will affect a camera or model matrix
    if(m_CameraEntityID != entityID) {
      // Set the model matrix with the global transform
      QuadRenderer::SetModelMatrix(globalTransform, m_QuadRenderers.at(entityID));
    }
    else {
      Camera::UpdateCameraPosition(globalTransform);
    }
    

    // Update the global transform of the children, and then recurse on the child transform
    for(unsigned int i : m_Children.at(entityID)) {

      m_GlobalTransforms.at(i).position = globalTransform.position;
      m_GlobalTransforms.at(i).rotation = globalTransform.rotation;
      m_GlobalTransforms.at(i).scale = globalTransform.scale;

      UpdateComponent(i,m_Transforms.at(i));
    }
  }

  void Scene::UpdateComponent(const unsigned int entityID, const QuadRendererComponent& replacement) {
    // Try to remove the old quad renderer and replace it with the new one. Then send an update 
    // quad call to the quad renderer so the updated data is reflected.
    try {
      m_QuadRenderers.extract(entityID);
    } 
    catch(const std::exception& e) {}
    m_QuadRenderers.insert({entityID,replacement});
    QuadRenderer::UpdateQuad(m_QuadRenderers.at(entityID));
  }

  void Scene::UpdateComponent(const unsigned int entityID, const BoxColliderComponent& replacement) {
    try {
      m_BoxColliders.extract(entityID);
    }
    catch(const std::exception& e){}
    m_BoxColliders.insert({entityID,replacement});
  }

  void Scene::UpdateComponent(const unsigned int entityID, const CircleColliderComponent& replacement) {
    try {
      m_CircleColliders.extract(entityID);
    }
    catch(const std::exception& e){}
    m_CircleColliders.insert({entityID,replacement});
  }

  TransformComponent Scene::GetTransform(const unsigned int entityID) {
    return m_Transforms.at(entityID);
  }

  QuadRendererComponent Scene::GetQuadRenderer(const unsigned int entityID) {
    return m_QuadRenderers.at(entityID);
  }

  BoxColliderComponent Scene::GetBoxCollider(const unsigned int entityID) {
    return m_BoxColliders.at(entityID);
  }

  CircleColliderComponent Scene::GetCircleCollider(const unsigned int entityID) {
    return m_CircleColliders.at(entityID);
  }
  
  bool Scene::CheckCollides(const unsigned int entityID) const {
    //TODO
    return false;
  }

  bool Scene::CheckCollides(const unsigned int entityID, const unsigned int otherEntityID) const {

    try {

      TransformComponent t = m_Transforms.at(entityID);
      TransformComponent global = m_GlobalTransforms.at(entityID);
      t.position += global.position;
      t.rotation += global.rotation;
      t.scale *= global.scale;

      TransformComponent otherT = m_Transforms.at(otherEntityID);
      TransformComponent otherGlobal = m_GlobalTransforms.at(otherEntityID);
      otherT.position += otherGlobal.position;
      otherT.rotation += otherGlobal.rotation;
      otherT.scale *= otherGlobal.scale;

      // If box colliders are found for both entities
      if(m_BoxColliders.find(entityID) != m_BoxColliders.cend() && m_BoxColliders.find(otherEntityID) != m_BoxColliders.cend()) {
        return Collision::Collides(m_BoxColliders.at(entityID), t, m_BoxColliders.at(otherEntityID), otherT);
      }

      // If circle colliders are found for both entities
      else if(m_CircleColliders.find(entityID) != m_CircleColliders.cend() && m_CircleColliders.find(otherEntityID) != m_CircleColliders.cend()) {
        return Collision::Collides(m_CircleColliders.at(entityID), t, m_CircleColliders.at(otherEntityID), otherT);
      }

      // TODO add box-circle collision

    }
    catch(const std::exception& e) {
      return false; // If an exception occurs somewhere then return false
    }

    // If this is somehow reached, return false
    return false;
  }
  

}