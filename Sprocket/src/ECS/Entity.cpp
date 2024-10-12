#include "ECS/Entity.h"

#include <stdexcept>
#include <algorithm>

namespace Sprocket {

  //////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////// ENTITYNODE FUNCTIONS //////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////

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

  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////// ENTITY TREE FUNCTIONS /////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  
  Entity::Entity(EntityNode* const parent) : EntityNode(false), m_Transform(TransformComponent()) {
    if(parent == nullptr) {
      throw std::invalid_argument("A parent must be a valid entity.");
    }
    m_Parent = parent;
    m_Parent->AddChild(this);
  }

  Entity::~Entity() {
    for(Entity* e : m_Children) {
      e->SetParent(m_Parent);
    }
    m_Parent->RemoveChild(this);

    // Destruct all components
    for(Component* c : m_Components) {
      delete c;
    }
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

  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////// COMPONENT FUNCTIONS //////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////

  unsigned int Entity::AddComponent(const Component& component) {

    if(component.GetComponentType() == ComponentType::TRANSFORM_COMPONENT) {
      throw std::invalid_argument("TransformComponents exist by default on Entities. More can not added.");
    }

    // Iterate through the vector to see if there are any empty spaces
    // TODO add a counter for number of components that are actually in the vector to know if this 
    // will be fruitful
    for(int i = 0; i < m_Components.size(); i++) {
      if(m_Components.at(i) == nullptr) {
        m_Components.at(i) = new Component(component); // Make a copy of the component to store
        return i;
      }
    }

    // If no empty spaces were found, add this component at the end of the vector
    m_Components.push_back(new Component(component)); // Make a copy of the component to store
    return m_Components.size()-1;

  }

  void Entity::RemoveComponent(const unsigned int id) {
    try {
    
      if(m_Components.at(id) == nullptr) {
        throw std::invalid_argument("The component at the given id has already been deleted.");
      }

      // Make the component that is being deleted a DeletedComponent which has no use other than to
      // indicate that it is a free id in the vector.
      m_Components.at(id) = nullptr;
    }
    catch(const std::out_of_range& e) {
      throw std::invalid_argument("The given id does not correspond to a valid component.");
    }
  }

  Component& Entity::GetComponent(const unsigned int id) {
    try {

      if(m_Components.at(id) == nullptr) {
        throw std::invalid_argument("The component at the given id has been deleted.");
      }

      return (Component&)*m_Components.at(id);
    }
    catch(const std::out_of_range& e) {
      throw std::invalid_argument("The given id does not correspond to a valid component.");
    }
  }

  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////

}