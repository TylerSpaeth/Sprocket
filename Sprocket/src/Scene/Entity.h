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

    public:

      Entity();
      ~Entity();

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