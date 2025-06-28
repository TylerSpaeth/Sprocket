#ifndef ENTITY_H
#define ENTITY_H

#include "Core/Macros.h"

#include "Events/Event.h"

#include "Component.h"
#include "TransformComponent.h"

#include <vector>
#include <typeinfo>

namespace Sprocket {

  class SPROCKET_API Entity {

    friend class Scene;

    private:

      Entity* m_Parent = nullptr;

      std::vector<Entity*> m_Children;

      std::vector<Component> m_Components;

      TransformComponent m_Transform;

      // Returns a transform representing the global of this entities parent.. The local values
      // are all the matter on this component. Global values are invalid.
      TransformComponent GetParentGlobalTransform();

      /// @brief Handles incoming events. Does not need to be registered as a callback. Should 
      /// instead be called directly be the Scene when it recieves an event.
      /// @param event The event the should be handled.
      void OnEvent(Event& event);

      // Note that these three functions below are called by the scene that they are a part of. 
      // As part of each of these functions, they will call Start(), Update(), and End() which 
      // are the public, overridable functions meant for user customization of the entities 
      // behavior.

      /// @brief This is called when the scene this entity is a part of becomes active or this
      /// entity is added to an active scene
      void OnActivate();
      /// @brief This is called when the scene this entity is a part of is no longer the active 
      /// scene or this entity is removed from an active scene
      void OnDeactivate();
      /// @brief This corresponds to application updates.
      /// @param deltaTime The time since the last update
      void OnUpdate(float deltaTime);

    public:

      Entity();
      ~Entity();

      virtual void Start(){}
      virtual void Update(float deltaTime){}
      virtual void End(){}

      template<typename T>
      bool AddComponent() {
        return false;
      }

      template<typename T>
      T* GetComponent() {
        return nullptr;
      }

      template<typename T>
      bool RemoveComponent() {
        return false;
      }

  };

  // Override GetComponent for components not stored in m_Components

  template<> 
  inline TransformComponent* Entity::GetComponent<TransformComponent>() {
    return &m_Transform;
  }

}

#endif