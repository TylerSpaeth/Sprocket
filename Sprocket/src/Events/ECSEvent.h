#ifndef ECSEVENT_H
#define ECSEVENT_H

#include "Event.h"
#include "ECS/Component.h"

#include <optional>

// TODO test
namespace Sprocket {

  enum class ECSEventType {
    NONE,

    ADD_TRANSFORM_COMPONENT,
    GET_TRANSFORM_COMPONENT,
    UPDATE_TRANSFORM_COMPONENT,
    DELETE_TRANSFROM_COMPONENT,

    ADD_CAMERA_COMPONENT,
    GET_CAMERA_COMPONENT,
    UPDATE_CAMERA_COMPONENT,
    DELETE_CAMERA_COMPONENT,

    ADD_QUADRENDERER_COMPONENT,
    GET_QUADRENDERER_COMPONENT,
    UPDATE_QUADRENDERER_COMPONENT,
    DELETE_QUADRENDERER_COMPONENT,

    ADD_PHYSICS_COMPONENT,
    GET_PHYSICS_COMPONENT,
    UPDATE_PHYSICS_COMPONENT,
    DELETE_PHYSICS_COMPONENT,

    ADD_TILEMAP_COMPONENT,
    GET_TILEMAP_COMPONENT,
    UPDATE_TILEMAP_COMPONENT,
    DELETE_TILEMAP_COMPONENT
  };

  enum class ECSEventCategory {
    UNCATEGORIZED, // Every event type can be classified as uncategorized
      TRANSFORM,
      CAMERA,
      QUADRENDERER,
      PHYSICS,
      TILEMAP
  };

  class ECSEvent : public Event {

    protected:

      ECSEventType m_EventType;

      ECSEvent(ECSEventType eventType, int entityID) : m_EventType(eventType), m_EntityID(entityID) {
        this->SetType(EventType::ECS);
      }

    public:

      int m_EntityID;

      ECSEventType GetEventType() {return m_EventType;}

      bool IsCategory(ECSEventCategory eventCategory) {

        if(eventCategory == ECSEventCategory::UNCATEGORIZED) {
          return true;
        }

        switch(m_EventType) {

          case ECSEventType::ADD_TRANSFORM_COMPONENT:
          case ECSEventType::GET_TRANSFORM_COMPONENT:
          case ECSEventType::UPDATE_TRANSFORM_COMPONENT:
          case ECSEventType::DELETE_TRANSFROM_COMPONENT:
            return eventCategory == ECSEventCategory::TRANSFORM;

          case ECSEventType::ADD_CAMERA_COMPONENT:
          case ECSEventType::GET_CAMERA_COMPONENT:
          case ECSEventType::UPDATE_CAMERA_COMPONENT:
          case ECSEventType::DELETE_CAMERA_COMPONENT:
            return eventCategory == ECSEventCategory::CAMERA;

          case ECSEventType::ADD_QUADRENDERER_COMPONENT:
          case ECSEventType::GET_QUADRENDERER_COMPONENT:
          case ECSEventType::UPDATE_QUADRENDERER_COMPONENT:
          case ECSEventType::DELETE_QUADRENDERER_COMPONENT:
            return eventCategory == ECSEventCategory::QUADRENDERER;

          case ECSEventType::ADD_PHYSICS_COMPONENT:
          case ECSEventType::GET_PHYSICS_COMPONENT:
          case ECSEventType::UPDATE_PHYSICS_COMPONENT:
          case ECSEventType::DELETE_PHYSICS_COMPONENT:
            return eventCategory == ECSEventCategory::PHYSICS;

          case ECSEventType::ADD_TILEMAP_COMPONENT:
          case ECSEventType::GET_TILEMAP_COMPONENT:
          case ECSEventType::UPDATE_TILEMAP_COMPONENT:
          case ECSEventType::DELETE_TILEMAP_COMPONENT:
            return eventCategory == ECSEventCategory::TILEMAP;
        }
      }
  };

  //////////////////////////////////////////// TRANSFORM //////////////////////////////////////////

  class AddTransformComponentEvent : public ECSEvent {
    public:
      TransformComponent m_Component; // The component to add
      AddTransformComponentEvent(int entityID, TransformComponent tcomp) : 
          ECSEvent(ECSEventType::ADD_TRANSFORM_COMPONENT, entityID), m_Component(tcomp) {}
  };
  class GetTransfromComponentEvent : public ECSEvent {
    public:
      std::optional<TransformComponent> m_Component; // Where the retrieved component will be
      GetTransfromComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::GET_TRANSFORM_COMPONENT, entityID) {}
  };
  class UpdateTransformComponentEvent : public ECSEvent {
    public:
      TransformComponent m_Component; // The component to use as the replacement
      UpdateTransformComponentEvent(int entityID, TransformComponent tcomp) : 
          ECSEvent(ECSEventType::UPDATE_TRANSFORM_COMPONENT, entityID), m_Component(tcomp) {}
  };
  class DeleteTransformComponentEvent : public ECSEvent {
    public:
      DeleteTransformComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::DELETE_TRANSFROM_COMPONENT, entityID) {}
  };

  ///////////////////////////////////////////// CAMERA ////////////////////////////////////////////
 
  class AddCameraComponentEvent : public ECSEvent {
    public:
      CameraComponent m_Component; // The component to add
      AddCameraComponentEvent(int entityID, CameraComponent ccomp) : 
          ECSEvent(ECSEventType::ADD_CAMERA_COMPONENT, entityID), m_Component(ccomp) {}
  };
  class GetCameraComponentEvent : public ECSEvent {
    public:
      std::optional<CameraComponent> m_Component; // Where the retrieved component will be
      GetCameraComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::GET_CAMERA_COMPONENT, entityID) {}
  };
  class UpdateCameraComponentEvent : public ECSEvent {
    public:
      CameraComponent m_Component; // The component to use as the replacement
      UpdateCameraComponentEvent(int entityID, CameraComponent ccomp) : 
          ECSEvent(ECSEventType::UPDATE_CAMERA_COMPONENT, entityID), m_Component(ccomp) {}
  };
  class DeleteCameraComponentEvent : public ECSEvent {
    public:
      DeleteCameraComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::DELETE_CAMERA_COMPONENT, entityID) {}
  };

  ////////////////////////////////////////// QUADRENDERER /////////////////////////////////////////

  class AddQuadRendererComponentEvent : public ECSEvent {
    public:
      QuadRendererComponent m_Component; // The component to add
      AddQuadRendererComponentEvent(int entityID, QuadRendererComponent qcomp) : 
          ECSEvent(ECSEventType::ADD_QUADRENDERER_COMPONENT, entityID), m_Component(qcomp) {}
  };
  class GetQuadRendererComponentEvent : public ECSEvent {
    public:
      std::optional<QuadRendererComponent> m_Component; // Where the retrieved component will be
      GetQuadRendererComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::GET_QUADRENDERER_COMPONENT, entityID) {}
  };
  class UpdateQuadRendererComponentEvent : public ECSEvent {
    public:
      QuadRendererComponent m_Component; // The component to use as the replacement
      UpdateQuadRendererComponentEvent(int entityID, QuadRendererComponent qcomp) : 
          ECSEvent(ECSEventType::UPDATE_QUADRENDERER_COMPONENT, entityID), m_Component(qcomp) {}
  };
  class DeleteQuadRendererComponentEvent : public ECSEvent {
    public:
      DeleteQuadRendererComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::DELETE_QUADRENDERER_COMPONENT, entityID) {}
  };

  ///////////////////////////////////////////// PHYSICS ///////////////////////////////////////////

  class AddPhysicsComponentEvent : public ECSEvent {
    public:
      PhysicsComponent m_Component; // The component to add
      ColliderComponent* m_Collider = nullptr; // An optional parameter for a collider
      AddPhysicsComponentEvent(int entityID, PhysicsComponent pcomp) : 
          ECSEvent(ECSEventType::ADD_PHYSICS_COMPONENT, entityID), m_Component(pcomp) {}
      AddPhysicsComponentEvent(int entityID, PhysicsComponent pcomp, ColliderComponent* ccomp) : 
          ECSEvent(ECSEventType::ADD_PHYSICS_COMPONENT, entityID), 
              m_Component(pcomp), 
              m_Collider (ccomp) {}
  };
  class GetPhysicsComponentEvent : public ECSEvent {
    public:
      std::optional<PhysicsComponent> m_Component; // Where the retrieved component will be
      std::optional<ColliderComponent> m_Collider; // Where the retrieved collider will be (optional)
      GetPhysicsComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::GET_PHYSICS_COMPONENT, entityID) {}
  };
  class UpdatePhysicsComponentEvent : public ECSEvent {
    public:
      PhysicsComponent m_Component; // The component to use as the replacement
      ColliderComponent* m_Collider = nullptr; // The collider to use as a replacement
      UpdatePhysicsComponentEvent(int entityID, PhysicsComponent pcomp) : 
          ECSEvent(ECSEventType::UPDATE_PHYSICS_COMPONENT, entityID), m_Component(pcomp) {}
      UpdatePhysicsComponentEvent(int entityID, PhysicsComponent pcomp, ColliderComponent* ccomp) : 
          ECSEvent(ECSEventType::UPDATE_PHYSICS_COMPONENT, entityID), 
              m_Component(pcomp), 
              m_Collider(ccomp) {}
  };
  class DeletePhysicsComponentEvent : public ECSEvent {
    public:
      DeletePhysicsComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::DELETE_PHYSICS_COMPONENT, entityID) {}
  };

  ///////////////////////////////////////////// TILEMAP ///////////////////////////////////////////

  class AddTileMapComponentEvent : public ECSEvent {
    public:
      TileMapComponent m_Component; // The component to add
      AddTileMapComponentEvent(int entityID, TileMapComponent tcomp) : 
          ECSEvent(ECSEventType::ADD_TILEMAP_COMPONENT, entityID), m_Component(tcomp) {}
  };
  class GetTileMapComponentEvent : public ECSEvent {
    public:
      std::optional<TileMapComponent> m_Component; // Where the retrieved component will be
      GetTileMapComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::GET_TILEMAP_COMPONENT, entityID) {}
  };
  class UpdateTileMapComponentEvent : public ECSEvent {
    public:
      TileMapComponent m_Component; // The component to use as the replacement
      UpdateTileMapComponentEvent(int entityID, TileMapComponent tcomp) : 
          ECSEvent(ECSEventType::UPDATE_TILEMAP_COMPONENT, entityID), m_Component(tcomp) {}
  };
  class DeleteTileMapComponentEvent : public ECSEvent {
    public:
      DeleteTileMapComponentEvent(int entityID) : 
          ECSEvent(ECSEventType::DELETE_TILEMAP_COMPONENT, entityID) {}
  };

}

#endif