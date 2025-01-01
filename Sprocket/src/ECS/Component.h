#ifndef COMPONENT_H
#define COMPONENT_H

#include "Core/Macros.h"

#include "ECS/ScriptableComponent.h"

#include "ThirdParty/glm/glm.hpp"

#include <string>
#include <array>

namespace Sprocket {

  struct TransformComponent {
    public:
      glm::vec3 position = glm::vec3(0,0,0);
      glm::vec3 rotation = glm::vec3(0,0,0);
      glm::vec3 scale = glm::vec3(1,1,1);
  };

  struct SpriteComponent {
    public:
      // If this is not assigned or set to "", then the sprite component will not be used
      std::string texturePath;
      // Size of the texture in the texture path
      float xDimension = 1;
      float yDimension = 1;
      // These coordinates start in the top right corner and go clockwise around the quad
      glm::vec4 xUVCoords = {1,1,0,0};
      glm::vec4 yUVCoords = {1,0,0,1};
  };

  // Right now, with the way other systems are setup, size can not be change after the component is
  // added to an entity. 
  struct QuadRendererComponent {
    friend class Scene;
    friend class QuadRenderer;
    friend class TileMap;
    private:
      int quadID = -1;
      float size;
    public:
      SpriteComponent sprite;
      glm::vec4 quadColor = {1,1,1,1};
      QuadRendererComponent(float size) : size(size){}
  };

  struct CameraComponent {

  };

  struct ColliderComponent {
    friend class Scene;
    friend class Physics;
    friend class PhysicsObject;
    friend class TileMap;
    protected:
      bool isBoxCollider;
      ColliderComponent(bool isBoxCollider) : isBoxCollider(isBoxCollider){}
  };

  struct BoxColliderComponent : public ColliderComponent {
    friend class Scene;
    friend class Physics;
    friend class TileMap;
    public:
      float width;
      float height;
      BoxColliderComponent() : ColliderComponent(true){}
  };

  struct CircleColliderComponent : public ColliderComponent {
    friend class Scene;
    friend class Physics;
    friend class TileMap;
    public:
      float radius;
      CircleColliderComponent() : ColliderComponent(false){}
  };

  struct PhysicsComponent {
    friend class Scene;
    friend class Physics;
    friend class TileMap;
    private:
      // This ID is used to reference a physic object in the physics system
      int physicsID = -1;
    public:
      // PhysicsComponents that are marked as dynamic are expected to move and is the default.
      // If set to false, then it is not expected to move.
      bool isDynamic = true;
  };

  struct TileMapComponent {

    friend class Scene;
    friend class TileMap;

    // In both the spriteMap and collider map, the first line of the file is reserved to denote the 
    // the row and column of the tile that should be the origin of the TileMap. They should be 
    // entered as 'row-column'. Note that this used 0 based indexing, with the tile at 0-0 being 
    // the top left tile in the map.

    // In a spriteMap, up to 10 textures can be used in a single TileMapComponent. In the file,
    // the characters 0-9 should be used to correspond to the 10 possible texturePaths that can be 
    // used. For spots that should not have a texture, an 'x' should be placed.

    // In the colliderMap, '0' should be used to denote that a box collider should be placed. 'x' 
    // should be used to denote that no collider belongs there. Note that tilemaps only support 
    // BoxColliderComponents that are the same size as the Tile they reside in.

    private:
      int tilemapID = -1;
    public:

      float tileSize;

      // Path to a file contain a map of how sprites should be laid out
      std::string spriteMapPath;
      // Path to a file contain a map of how colliders should be laid out
      std::string colliderMapPath;
      // An array of sprites to be used to render the spriteMapPath.
      std::array<SpriteComponent, 10> sprites;
  };

  struct ScriptComponent {

    ScriptableComponent* instance = nullptr;

    ScriptableComponent*(*InstantiateScript)();
    void(*DestroyScript)(ScriptComponent*);

    template<typename T>
    void Bind() {
      InstantiateScript = []() {return static_cast<ScriptableComponent*>(new T());};
      DestroyScript = [](ScriptComponent* sc) {delete sc->instance; sc->instance = nullptr;};
    }

  };
  


}

#endif