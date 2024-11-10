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

  Scene::Scene() : m_Physics(new Physics()), m_QuadRenderer(new QuadRenderer()), m_Camera(new Camera()) {
    // TODO this is really not optimal. It would be best not to have this be a fixed number, and 
    // definitly not hard coded here. If reserving space for the vector is the way this stays, then
    // it would probably be best to set this number to the same as the maximum number of quads. It 
    // may even be worth it at this point, to stick with maps and take the performance hit at scale
    // for more efficient memory usage
    m_Transforms.reserve(100000);
    m_GlobalTransforms.reserve(100000);
  }

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

    // If there is a free spot in the vector of transforms
    if(m_DeletedEntities.size()) {

      // Get the index of the slot
      auto index = m_DeletedEntities.top();
      m_DeletedEntities.pop();

      // Reset the local transform
      m_Transforms.at(index).position = {0,0,0};
      m_Transforms.at(index).rotation = {0,0,0};
      m_Transforms.at(index).scale = {1,1,1};

      // Reset the global transform
      m_GlobalTransforms.at(index).position = {0,0,0};
      m_GlobalTransforms.at(index).rotation = {0,0,0};
      m_GlobalTransforms.at(index).scale = {1,1,1};

    }

    m_Transforms.push_back(TransformComponent());
    m_GlobalTransforms.push_back(TransformComponent());
    m_Children.push_back(std::vector<unsigned int>());
    m_Parents.push_back(-1);
    return m_EntityCount++;
  }

  void Scene::DeleteEntity(const unsigned int entityID) {

    try {
      // See if there is a transfrom for this entity, and if there is, add this id to the queue of deleted ids
      m_Transforms.at(entityID);
      m_DeletedEntities.push(entityID);
    } 
    // If there is not transform for this entity, then the entity does not exist
    catch(const std::exception& e) {
      return;
    }

    // Try to delete the quad renderer
    if(m_QuadRenderers.count(entityID)) {
      ((QuadRenderer*)m_QuadRenderer)->DeleteQuad(m_QuadRenderers.at(entityID));
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

    ((QuadRenderer*)m_QuadRenderer)->RenderNewQuad(m_Transforms.at(entityID), m_QuadRenderers.at(entityID));
    ((QuadRenderer*)m_QuadRenderer)->UpdateQuad(m_QuadRenderers.at(entityID));

    TransformComponent globalTransform = m_GlobalTransforms.at(entityID);
    TransformComponent localTransform = m_Transforms.at(entityID);
    globalTransform.position += localTransform.position;
    globalTransform.rotation += localTransform.rotation;
    globalTransform.scale *= localTransform.scale;

    ((QuadRenderer*)m_QuadRenderer)->SetModelMatrix(globalTransform, m_QuadRenderers.at(entityID));
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
      ((QuadRenderer*)m_QuadRenderer)->SetModelMatrix(globalTransform, m_QuadRenderers.at(entityID));
    }
    else if(m_CameraEntityID == entityID) {
      ((Camera*)m_Camera)->UpdateCameraPosition(globalTransform);
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
      ((QuadRenderer*)m_QuadRenderer)->UpdateQuad(m_QuadRenderers.at(entityID));
    }
    
  }

  void Scene::UpdateComponent(const unsigned int entityID, const BoxColliderComponent& replacement) {
    if(m_BoxColliders.count(entityID)) {
      m_BoxColliders.at(entityID).height = replacement.height;
      m_BoxColliders.at(entityID).width = replacement.width;
    }
  }

  void Scene::UpdateComponent(const unsigned int entityID, const CircleColliderComponent& replacement) {
    if(m_CircleColliders.count(entityID)) {
      m_CircleColliders.at(entityID).radius = replacement.radius;
    }
  }

  // TODO This may need change in the future. Right now it checks to make sure that the new 
  // component has the same physicsID as the old component. This is not a fool proof solution.
  void Scene::UpdateComponent(const unsigned int entityID, const PhysicsComponent& component) {
    if(m_PhysicsComponents.count(entityID) && m_PhysicsComponents.at(entityID).phyiscsID == component.phyiscsID) {
      m_PhysicsComponents.at(entityID).isDynamic = component.isDynamic;
      m_PhysicsComponents.at(entityID).phyiscsID = component.phyiscsID;

    }
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////// PHYSICS FUNCTIONS ///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  bool Scene::CheckCollides(const unsigned int entityID) const {

    try {
      return ((Physics*)m_Physics)->CountCollisions(m_PhysicsComponents.at(entityID).phyiscsID);
    }
    // If an exception occurs, which would most likely happen trying to retrieve a physics component
    // return false since no collision could occur
    catch(const std::exception& e) {
      return false;
    }

  }

  bool Scene::CheckCollides(const unsigned int entityID, const unsigned int otherEntityID) const {

    try {
      return ((Physics*)m_Physics)->CollidesWith(m_PhysicsComponents.at(entityID).phyiscsID, m_PhysicsComponents.at(otherEntityID).phyiscsID);
    }
    // If an exception occurs, which would most likely happen trying to retrieve a physics component
    // return false since no collision could occur
    catch(const std::exception& e) {
      return false;
    }

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////// PRIVATE HELPERS ////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void Scene::RemoveQuadRenderer(const unsigned int entityID) {
    ((QuadRenderer*)m_QuadRenderer)->DeleteQuad(m_QuadRenderers.at(entityID));
  }

  void Scene::RemovePhysicsObjectCollider(const unsigned int entityID) {
    ((Physics*)m_Physics)->RemoveCollider(m_PhysicsComponents.at(entityID).phyiscsID);
  }

  void Scene::RemovePhysicsObject(const unsigned int entityID) {
    // Get the physicsID and delete the correspond physics object
    int id = m_PhysicsComponents.at(entityID).phyiscsID;
    ((Physics*)m_Physics)->DeletePhysicsObject(id);
  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void Scene::RegisterEventCallback(const std::function<void(Event&)> eventCallback) {
    m_EventCallback = eventCallback;
    ((QuadRenderer*)m_QuadRenderer)->m_EventCallback = eventCallback;
    ((Camera*)m_Camera)->m_EventCallback = eventCallback;
  }

}