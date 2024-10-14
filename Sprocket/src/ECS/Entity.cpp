#include "ECS/Entity.h"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

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

        // TODO implement this for every component type
        switch(component.GetComponentType()) {
          case Sprocket::ComponentType::TEST_COMPONENT: {
            TestComponent* toAdd = new TestComponent((const TestComponent&)component);
            toAdd->m_Entity = this;
            toAdd->OnAttach();
            m_Components.at(i) = toAdd;
            return i;
          }
          case Sprocket::ComponentType::QUAD_RENDERER: {
            QuadRenderer* toAdd = new QuadRenderer((const QuadRenderer&)component);
            toAdd->m_Entity = this;
            toAdd->OnAttach();
            m_Components.at(i) = toAdd;
            return i;
          }

        }
       
      }
    }

    // TODO implement this for every component type
    switch(component.GetComponentType()) {
      case Sprocket::ComponentType::TEST_COMPONENT: {
        TestComponent* toAdd = new TestComponent((const TestComponent&)component);
        toAdd->m_Entity = this;
        toAdd->OnAttach();
        m_Components.push_back(toAdd);
        return m_Components.size()-1;
      }
      case Sprocket::ComponentType::QUAD_RENDERER: {
        QuadRenderer* toAdd = new QuadRenderer((const QuadRenderer&)component);
        toAdd->m_Entity = this;
        toAdd->OnAttach();
        m_Components.push_back(toAdd);
        return m_Components.size()-1;
      }
    }

    return -1; // Indicates some sort of failure to add the component
  }

  void Entity::RemoveComponent(const unsigned int id) {
    try {
    
      if(m_Components.at(id) == nullptr) {
        throw std::invalid_argument("The component at the given id has already been deleted.");
      }

      // Make the component that is being deleted a nullptr to indicate that it is a free id in 
      // the vector.
      m_Components.at(id)->OnDetach();
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

  TransformComponent Entity::GetGlobalTransform() const {
    TransformComponent toReturn = m_Transform;
    
    Entity* parent = (Entity*)m_Parent;
    while(!parent->IsRoot()) {
      toReturn.m_Position += parent->m_Transform.m_Position;
      toReturn.m_Rotation += parent->m_Transform.m_Rotation;
      toReturn.m_Scale *= parent->m_Transform.m_Scale;

      parent = (Entity*)parent->m_Parent;
    }

    return toReturn;
  }

  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////

}