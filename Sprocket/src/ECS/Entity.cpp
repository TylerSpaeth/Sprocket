#include "ECS/Entity.h"

#include <stdexcept>
#include <algorithm>

namespace Sprocket {

  ///////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// PRIVATE FUNCTIONS ////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////

  void EntityNode::AddChild(Entity* const child) {
    if(child == this) {
      throw std::invalid_argument("An entity may not have itself as a child.");
    }
    if(child == nullptr) {
      throw std::invalid_argument("Children must be Entities.");
    }
    m_Children.push_back(child);
  }

  void EntityNode::RemoveChild(Entity* const child) {
    auto it = std::find(m_Children.begin(), m_Children.end(), child);
    if(it == m_Children.cend()) {
      throw std::invalid_argument("The given Entity is not a child of this Entity.");
    }
    m_Children.erase(it);
  }

  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////////////////////////////////////////////////////////
  
  Entity::Entity(EntityNode* const parent) : EntityNode(false) {
    if(parent == nullptr) {
      throw std::invalid_argument("A parent must be a valid entity.");
    }
    m_Parent = parent;
    m_Parent->AddChild(this);
  }

  Entity::~Entity() {
    // Set all children of this Entity to have the parent of this Entity
    for(Entity* e : m_Children) {
      e->SetParent(m_Parent);
    }
    // Remove this entity from the parents list of children
    m_Parent->RemoveChild(this);
  }

  void Entity::SetParent(EntityNode* const parent) {
    if(parent == nullptr) {
      throw std::invalid_argument("A parent must be a valid entity.");
    }
    if(parent == this) {
      throw std::invalid_argument("An entity may not be it's own parent.");
    }
    
    m_Parent->RemoveChild(this);
    m_Parent = parent;
    m_Parent->AddChild(this);
  }

  

}