#ifndef ENTITY_H
#define ENTITY_H

#include "Core/Macros.h"
#include "ECS/Component.h"

#include <vector>

namespace Sprocket {

  class Entity;
  class RootEntity;
  class EntityNode;

  class SPROCKET_API EntityNode {
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

      /// @brief Gives all of the children of this Entity.
      /// @return a vector of pointers to this Entity's children.
      const std::vector<Entity*> GetChildren() const {return m_Children;}

      /// @brief Returns whether or not this is a RootEntity or not
      /// @return true if this is a RootEntity, false othewise
      bool IsRoot() const {return m_IsRoot;}

  };
  
  class SPROCKET_API Entity : public EntityNode {
    private:
      EntityNode* m_Parent;
      
      //TransformComponent& m_Transform;
      //std::vector<Component*> m_Components;

    public:

      /// @brief Create a new Entity with the given parent. Parent may be specified as nullptr,
      /// but it recommended to only use that in the case of the root node in a scene tree.
      /// @param parent A pointer to the Entity that should be the parent of this new Entity.
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

      /// @brief Returns a pointer to the parent of this Entity.
      /// @return A pointer to the parent of this Entity
      EntityNode* const GetParent() const {return m_Parent;}

      //void AddComponent(Component& component);
      //RemoveComponent();
      //TransformComponent& GetTransform() const {return m_Transform;}

      
  };

  class RootEntity : public EntityNode {
    friend class Scene;
    private:
      RootEntity() : EntityNode(true) {}
  };  

} 

#endif