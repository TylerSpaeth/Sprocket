#include "ECS/Scene.h"
#include "ECS/QuadRenderer.h"
#include "ECS/Camera.h"
#include "ECS/Collision.h"
#include "ECS/Physics.h"

#include "Events/ApplicationEvent.h"

#include <iostream>
#include <algorithm>
#include <stdexcept>

namespace Sprocket {

  Scene::Scene() : m_Physics(new Physics()) {}

  Scene::~Scene() {
    delete (Physics*)m_Physics;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////// EVENT HANDLING /////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void Scene::OnEvent(Event& event) {
    switch(event.GetEventType()) {
      case EventType::APP_UPDATE:
        OnUpdate(((ApplicationUpdateEvent&)event).GetDeltaTime());
        break;
      case EventType::WINDOW_CLOSE:
        OnClose();
        break;
    }
    //TODO handle events
  }

  void Scene::OnUpdate(float deltaTime) {
    // Perform a physics update
    ((Physics*)m_Physics)->OnUpdate(deltaTime);
  }

  void Scene::OnClose() {

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////// SCENE TREE FUNCTIONS //////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  unsigned int Scene::CreateEntity() {
    m_Transforms.push_back(TransformComponent());
    m_GlobalTransforms.push_back(TransformComponent());
    m_Children.push_back(std::vector<unsigned int>());
    m_Parents.push_back(-1);
    return m_EntityCount++;
  }

  // FIXME right now this only deletes all of the components except for the transform. Need to finish this
  void Scene::DeleteEntity(const unsigned int entityID) {

    // Try to delete the quad renderer
    if(m_QuadRenderers.count(entityID)) {
      QuadRenderer::DeleteQuad(m_QuadRenderers.at(entityID));
      m_QuadRenderers.erase(entityID);
    }

    // Delete a collider if it has one
    if(m_BoxColliders.count(entityID)) {
      m_BoxColliders.extract(entityID);
    }
    else if(m_CircleColliders.count(entityID)) {
      m_CircleColliders.extract(entityID);
    }
    
    // If there is a physics component delete it from the physics system and remove it from the map
    if(m_PhysicsComponents.count(entityID)) {
      ((Physics*)m_Physics)->DeletePhysicsObject(m_PhysicsComponents.at(entityID).phyiscsID);
      m_PhysicsComponents.extract(entityID);
    }

    // If this has a camera component remove it
    if(m_CameraEntityID == entityID) {
      m_CameraEntityID = -1;
    }

    // Set the parent of all children to the parent of this entity
    for(auto child : m_Children.at(entityID)) {
      SetEntityParent(child, m_Parents.at(entityID));
    }

    SetEntityParent(entityID,-1);

  }

  // TODO there is a better way to do this, this is just going to work for now
  void Scene::SetEntityParent(const unsigned int entityID, const unsigned int parentID) {

    int currentParent = m_Parents.at(entityID);

    // If this entity has a parent, remove it from the parents children list
    if(currentParent != -1) {
      auto it = std::find(m_Children.at(currentParent).begin(), m_Children.at(currentParent).end(), entityID);
      m_Children.at(currentParent).erase(it);
    }

    // Set the parentID
    m_Parents.at(entityID) = parentID;

    // If attempting to set the entity to have no parent, makes its global transfrom default
    if(parentID == -1) {
      m_GlobalTransforms.at(entityID).position = {0,0,0};
      m_GlobalTransforms.at(entityID).position = {0,0,0};
      m_GlobalTransforms.at(entityID).scale = {1,1,1};
      return;
    }

    // All of the below only occurs if setting an actual parent
   
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

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////// ADDCOMPONENT FUNCTIONS /////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void Scene::AddComponent(const unsigned int entityID, const QuadRendererComponent& component) {
    
    if(m_QuadRenderers.count(entityID)) {
      throw std::invalid_argument("This entity already has a QuadRendererComponent. An entity may only hold a single quad renderer.");
    }

    if(m_CameraEntityID == entityID) {
      throw std::invalid_argument("This entity has a camera component attached and therefor can not have a QuadRendererComponent.");
    }
   
    m_QuadRenderers.insert({entityID,component});

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
      throw std::runtime_error("There is already a camera in this scene.");
    }

    if(m_QuadRenderers.count(entityID)) {
      throw std::invalid_argument("This entity has a QuadRendererComponent attached to it. Remove that before adding a CameraComponent.");
    }

    m_CameraEntityID = entityID;

    // TODO implement actual storage of a CameraComponent once the CameraComponent has actual 
    // data associated with it

  }

  void Scene::AddComponent(const unsigned int entityID, const BoxColliderComponent& component) {

    if(m_CircleColliders.find(entityID) != m_CircleColliders.cend()) {
      throw std::invalid_argument("This entity already has a circle collider. An entity may only have a single collider of any type.");
    }

    try {
      m_BoxColliders.at(entityID);
      throw std::invalid_argument("This entity already has a box collider. An entity may only have a single collider of any type.");
    }
    catch(const std::exception& e) {
      m_BoxColliders.insert({entityID,component});
      
      // Check if the entity already has a physics component, and if not, add one
      if(!m_PhysicsComponents.count(entityID)) {
        AddComponent(entityID,PhysicsComponent());
      }

      // If it already has a physics component, update set the collider of the corresponding 
      // physics object
      else {
        auto p = m_PhysicsComponents.at(entityID);
        ((Physics*)m_Physics)->SetCollider(p.phyiscsID,m_BoxColliders.at(entityID));
      }

    }
  }

  void Scene::AddComponent(const unsigned int entityID, const CircleColliderComponent& component) {

    if(m_BoxColliders.find(entityID) != m_BoxColliders.cend()) {
      throw std::invalid_argument("This entity already has a box collider. An entity may only have a single collider of any type.");
    }

    try {
      m_CircleColliders.at(entityID);
      throw std::invalid_argument("This entity already has a circle collider. An entity may only have a single collider of any type.");
    }
    catch(const std::exception& e) {
      m_CircleColliders.insert({entityID,component});

      // Check if the entity already has a physics component, and if not, add one
      if(!m_PhysicsComponents.count(entityID)) {
        AddComponent(entityID,PhysicsComponent());
      }
      // If it already has a physics component, update set the collider of the corresponding 
      // physics object
      else {
        auto p = m_PhysicsComponents.at(entityID);
        ((Physics*)m_Physics)->SetCollider(p.phyiscsID,m_CircleColliders.at(entityID));
      }

    }
  }

  void Scene::AddComponent(const unsigned int entityID, const PhysicsComponent& component) {
    if(m_PhysicsComponents.count(entityID)) {
      throw std::invalid_argument("This entity already has a PhysicsComponent. An entity may only hold a single PhysicsComponent.");
    }

    m_PhysicsComponents.insert({entityID,component});

    // Register the component with the physics system based on the other components of the entity
    if(m_BoxColliders.count(entityID)) {
      ((Physics*)m_Physics)->RegisterNewPhysicsObject(m_Transforms.at(entityID),m_GlobalTransforms.at(entityID), m_PhysicsComponents.at(entityID), m_BoxColliders.at(entityID));
    }
    else if(m_CircleColliders.count(entityID)) {
      ((Physics*)m_Physics)->RegisterNewPhysicsObject(m_Transforms.at(entityID),m_GlobalTransforms.at(entityID), m_PhysicsComponents.at(entityID), m_CircleColliders.at(entityID));
    }
    else {
      ((Physics*)m_Physics)->RegisterNewPhysicsObject(m_Transforms.at(entityID),m_GlobalTransforms.at(entityID), m_PhysicsComponents.at(entityID));
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////// UPDATECOMPONENT FUNCTIONS ///////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

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

    // Check whether this transform change will affect a camera or model matrix
    if(m_CameraEntityID != entityID && m_QuadRenderers.count(entityID)) {
      // Set the model matrix with the global transform
      QuadRenderer::SetModelMatrix(globalTransform, m_QuadRenderers.at(entityID));
    }
    else if(m_CameraEntityID == entityID) {
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
    if(m_QuadRenderers.count(entityID)) {
      m_QuadRenderers.extract(entityID);
      m_QuadRenderers.insert({entityID,replacement});
      QuadRenderer::UpdateQuad(m_QuadRenderers.at(entityID));
    }
    
  }

  void Scene::UpdateComponent(const unsigned int entityID, const BoxColliderComponent& replacement) {
    if(m_BoxColliders.count(entityID)) {
      m_BoxColliders.extract(entityID);
      m_BoxColliders.insert({entityID,replacement});
    }
  }

  void Scene::UpdateComponent(const unsigned int entityID, const CircleColliderComponent& replacement) {
    if(m_CircleColliders.count(entityID)) {
      m_CircleColliders.extract(entityID);
      m_CircleColliders.insert({entityID,replacement});
    }
  }

  // TODO This may need change in the future. Right now it checks to make sure that the new 
  // component has the same physicsID as the old component. This is not a fool proof solution.
  void Scene::UpdateComponent(const unsigned int entityID, const PhysicsComponent& component) {
    if(m_PhysicsComponents.count(entityID) && m_PhysicsComponents.at(entityID).phyiscsID == component.phyiscsID) {
      m_PhysicsComponents.extract(entityID);
      m_PhysicsComponents.insert({entityID,component});
    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////// PHYSICS FUNCTIONS ///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  bool Scene::CheckCollides(const unsigned int entityID) const {
    
    for(auto const& b : m_BoxColliders) {
      if(CheckCollides(entityID,b.first) && entityID != b.first) {
        return true;
      }
    }

    for(auto const& c : m_CircleColliders) {
      if(CheckCollides(entityID,c.first) && entityID != c.first) {
        return true;
      }
    }

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

      // Box-Circle
      else if(m_BoxColliders.find(entityID) != m_BoxColliders.cend() && m_CircleColliders.find(otherEntityID) != m_CircleColliders.cend()) {
        return Collision::Collides(m_BoxColliders.at(entityID), t, m_CircleColliders.at(otherEntityID), otherT);
      }

      // Circle-Box
      else if(m_CircleColliders.find(entityID) != m_CircleColliders.cend() && m_BoxColliders.find(otherEntityID) != m_BoxColliders.cend()) {
        return Collision::Collides(m_CircleColliders.at(entityID), t, m_BoxColliders.at(otherEntityID), otherT);
      }

    }
    catch(const std::exception& e) {
      return false; // If an exception occurs somewhere then return false
    }

    // If this is somehow reached, return false
    return false;
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  // TODO make sure this is valid to do
  void Scene::RemoveQuadRenderer(const unsigned int entityID) {
    QuadRenderer::DeleteQuad(m_QuadRenderers.at(entityID));
  }

}