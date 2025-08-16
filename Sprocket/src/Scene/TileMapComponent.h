#ifndef TILEMAPCOMPONENT_H
#define TILEMAPCOMPONENT_H

#include "Component.h"

#include "Events/Event.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {

  // FIXME for some reason rotating the tilemap has a major negative impact on performance.
  // Not sure what is causing this
  class SPROCKET_API TileMapComponent : public Component {

    friend class Entity;

    private:

      struct QuadRendererStruct {
        glm::vec4 m_QuadColor = {1,1,1,1};
        std::string m_TexturePath;
        // These coordinates start in the top right corner and go clockwise around the quad
        glm::vec4 m_TextureXUVCoords = {1,1,0,0};
        glm::vec4 m_TextureYUVCoords = {1,0,0,1};
      };

      glm::vec3 m_Position;
      glm::vec3 m_Rotation;
      glm::vec3 m_Scale;

      std::string m_QuadRendererMapPath;
      std::string m_ColliderMapPath;

      std::vector<unsigned int> m_PhysicsIDs;

      // QuadRenderer maps have a maximum number of unique tiles that are defined by the printable 
      // ascii characters. The ~ character is used to denote no sprite and can not be mapped.
      // The arrays are based on the first printable ascii character are sized to be able to map
      // all ascii characters except for ~.
      static constexpr char EMPTY_PLACEHOLDER_CHAR = '~';
      static constexpr char FIRST_PRINTABLE_ASCII = '!';
      static constexpr unsigned int MAX_UNIQUE_TILES = 94;
      std::array<std::vector<unsigned int>, MAX_UNIQUE_TILES> m_QuadRendererIDs;
      std::array<QuadRendererStruct, MAX_UNIQUE_TILES> m_QuadRenderers;

      std::function<void(Event&)> m_EventCallback;

      TileMapComponent() {}

      void RegisterTileMap(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
      void RegisterQuadRendererMap();
      void RegisterColliderMap();
      void UpdateTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
      void DeleteTileMap();
      void DeleteQuadRendererMap();
      void DeleteColliderMap();

    public:

      void SetQuadRendererMapPath(const std::string quadRendererMapPath);
      void SetColliderMapPath(const std::string colliderMapPath);

      // The index for these functions in the spot in the QuadRenderers array they should update
      bool SetQuadRendererData(const char index, const glm::vec4 quadColor);
      bool SetQuadRendererData(const char index, const std::string texturePath);
      bool SetQuadRendererData(const char index, const std::string texturePath, const glm::vec4 textureXUVCoords, const glm::vec4 textureYUVCoords);

  };
 
}

#endif