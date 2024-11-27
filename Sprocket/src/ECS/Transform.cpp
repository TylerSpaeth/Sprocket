#include "Transform.h"

namespace Sprocket {

  Transform::Transform() {

    // 0 will always be the first available index in a new vector
    m_FreeTransforms.push(0);
    
  }

  void Transform::OnEvent(Event& event) {

    switch(event.GetEventType()) {
      case EventType::APP_UPDATE:
        // TODO handle any logic that needs to happen with every update cycle
        break;
      case EventType::ECS:
        OnECSEvent((ECSEvent&)event);
        break;
    }
  }

  void Transform::OnECSEvent(ECSEvent& event) {

    switch(event.GetEventType()) {

      case ECSEventType::ADD_TRANSFORM_COMPONENT: {

        auto entityID = ((AddTransformComponentEvent&)event).m_EntityID;
        auto comp = ((AddTransformComponentEvent&)event).m_Component;
        RegisterNewTransform(entityID, comp);
        break;
      }
      case ECSEventType::GET_TRANSFORM_COMPONENT: {

        // Verify that the given entityID corresponds to a valid TransformComponent
        if(m_EntityMapping.count(event.m_EntityID) == 0) return;

        // Assign the component value which is used as a return field for this event type
        ((GetTransfromComponentEvent&)event).m_Component = GetTransform(event.m_EntityID);
        break;
      }
      case ECSEventType::UPDATE_TRANSFORM_COMPONENT: {

        auto entityID = ((UpdateTransformComponentEvent&)event).m_EntityID;
        auto comp = ((UpdateTransformComponentEvent&)event).m_Component;
        UpdateTransform(entityID, comp);
        break;
      }
      case ECSEventType::DELETE_TRANSFROM_COMPONENT: {

        DeleteTransform(event.m_EntityID);
        break;
      }

    }
  }

  unsigned int Transform::GetFreeSlot() {

    auto top = m_FreeTransforms.top();
    
    // If there is only one free transform, then it must be at the end of the vector, so we need
    // to add the next free slot as being 1 slot after the current free slot.
    if(m_FreeTransforms.size() == 1) {
      m_FreeTransforms.push(top+1);
    }

    m_FreeTransforms.pop();

    return top;

  }

  bool Transform::RegisterNewTransform(int entityID, TransformComponent& tcomp) {

    // Ensure the entityID does not already have a TransformComponent
    if(m_EntityMapping.count(entityID) != 0) return false;

    unsigned int freeSlot = GetFreeSlot();

    // Create a new entry in the entity mapping, mapping the entityID to the slot in which the 
    // transform will be stored.
    m_EntityMapping.insert({entityID,freeSlot});

    // If the freeSlot is not at the end, then replace the values in that slot, otherwise push the 
    // component to the end of the vector
    if(freeSlot < m_LocalTransforms.size()) {
      m_LocalTransforms.at(freeSlot) = tcomp;
    }
    else {
      m_LocalTransforms.push_back(tcomp);
    }

    return true;

  }

  TransformComponent Transform::GetTransform(int entityID) const {

    return m_LocalTransforms.at(m_EntityMapping.at(entityID));

  }

  bool Transform::UpdateTransform(int entityID, TransformComponent& tcomp) {
    
    // Verify that the given entityID corresponds to a valid TransformComponent
    if(m_EntityMapping.count(entityID) == 0) return false;

    m_LocalTransforms.at(m_EntityMapping.at(entityID)) = tcomp;

    return true;
  }

  bool Transform::DeleteTransform(int entityID) {

    // TODO implement maintance to clear values at the end of the vector if the last element in the 
    // transform vector has been deleted.

    // The actual values in the transform vector are not removed

    // Verify that the given entityID corresponds to a valid TransformComponent
    if(m_EntityMapping.count(entityID) == 0) return false;

    // Add this slot to the free queue
    m_FreeTransforms.push(m_EntityMapping.at(entityID));

    // Remove the mapping so it can no longer be used by this entityID.
    m_EntityMapping.extract(entityID);

    return true;
  }

}