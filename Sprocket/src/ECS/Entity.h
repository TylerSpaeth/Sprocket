#ifndef ENTITY_H
#define ENTITY_H

#include "Core/Macros.h"
#include "ECS/Component.h"
#include "Events/Event.h"

#include <vector>

namespace Sprocket {

  class Entity;
  class RootEntity;
  class EntityNode;

  class EntityNode {

    friend class Entity;

    protected:

      const bool m_IsRoot;
      std::vector<Entity*> m_Children;

      EntityNode(bool isRoot) : m_IsRoot(isRoot){}

      // These two functions should only be used by Entity class using it's SetParent function.

      /// @brief Adds the given Entity as a child of this entity.
      /// @param child The entity to add as a child of this Entity.
      /// @throws std::invalid_argument if the given child is this Entity or nullptr.
      void AddChild(Entity* const child);
        
      /// @brief Removes the given Entity from the list of children.
      /// @param child The child to be removed from the Entities children.
      /// @throws std::invalid_argument if the given Entity is not a child of this Entity.
      void RemoveChild(Entity* const child);

    public:

      const std::vector<Entity*> GetChildren() const {return m_Children;}
      bool IsRoot() const {return m_IsRoot;}

  };
  
  class SPROCKET_API Entity : public EntityNode {

    private:

      EntityNode* m_Parent;
      
      // Transform is a special component that is attached to every entity and can not be removed 
      // and additional transforms can not be added
      TransformComponent m_Transform;
      std::vector<Component*> m_Components;

    public:

      /// @brief Create a new Entity with the given parent. Parent may be specified as nullptr,
      /// but it recommended to only use that in the case of the root node in a scene tree.
      /// @param parent A pointer to the Entity that should be the parent of this new Entity.
      /// @throws std::invalid_argument if the given parent is nullptr.
      Entity(EntityNode* const parent);

      /// @brief Removes this Entity from any tree it is a part of. Its children are passed on to 
      /// its parent and this is no longer a child of the parent. This also destructs the Entity, 
      /// so any subsequent function calls to this class instance after calling this function are
      /// undefined.
      ~Entity();
      
      /// @brief Sets the parent of this Entity to the given pointer. This may not be nullptr, that 
      /// ability is reserved exclusively for instantiation. Also if a parent is instantiated to
      /// to nullptr then the parent may not be changed later. This is the function that allows a 
      /// tree of entities to be reordered. 
      /// @param parent A pointer to the Entity that should be the parent of this Entity.
      /// @throws std::invalid_argument In the following two cases:
      ///   1. The given parent is nullptr.
      ///   2. The given parent is THIS Entity.
      void SetParent(EntityNode* const parent);

      EntityNode* const GetParent() const {return m_Parent;}

      /// @brief Adds a new component to this Entity.
      /// @param component The component that should be added.
      /// @return an id that allows retreival and deletion of the component from this Entity.
      /// @throws std::invalid_argument if the given component can not be added.
      unsigned int AddComponent(const Component& component);

      /// @brief Removes the component with the given id.
      /// @param id The id of the component to be removed.
      /// @throws std::invalid_argument if the given id does not correspond to a valid component.
      void RemoveComponent(const unsigned int id);

      /// @brief Gets the component at the given id.
      /// @param id The id of the component to be retreived
      /// @return A reference to the component at the index.
      /// @throws std::invalid_argument if the id does not correspond to a valid component.
      Component& GetComponent(const unsigned int id);

      TransformComponent& GetLocalTransform() {return m_Transform;}
      TransformComponent GetGlobalTransform() const;
 
  };

  class RootEntity : public EntityNode {

    friend class Scene;

    private:
      RootEntity() : EntityNode(true) {}
      // If a root entity is destroyed, destroy all children first
      ~RootEntity() {
        while(m_Children.size() != 0) {
          delete m_Children.at(0);
        }
      }
  };  

} 

#endif