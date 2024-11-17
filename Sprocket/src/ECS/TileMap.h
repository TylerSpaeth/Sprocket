#ifndef TILEMAP_H
#define TILEMAP_H

#include "ECS/Component.h"
#include "ECS/QuadRenderer.h"
#include "ECS/Physics.h"

#include <queue>
#include <vector>
#include <map>

namespace Sprocket {

  class TileMap {
    friend class Scene;
    private:

      QuadRenderer* m_QuadRenderer;
      Physics* m_Physics;

      std::map<unsigned int, std::map<unsigned int, std::tuple<TransformComponent*,TransformComponent*,PhysicsComponent*,ColliderComponent*>>> m_PhysicsPointers;
      std::map<unsigned int, std::vector<int>> m_QuadRendererIDs;

      // Stores the free tilemapIDs from lowest to highest
      std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_FreeIDs;

      TileMap(QuadRenderer* quadRenderer, Physics* physics);

      void RegisterTileMap(TransformComponent transform, TileMapComponent& tilemap);
      void DeleteTileMap(const int tilemapID);

      void GenerateTileMapID(TileMapComponent& tilemap);
      void IntializeTileMapStorage(TileMapComponent& tilemap);

      // Helpers
      void RegisterSpriteMap(TransformComponent transform, TileMapComponent& tilemap);
      void RegisterColliderMap(TransformComponent transform, TileMapComponent& tilemap);
      void DeleteSpriteMap(const int tilemapID);
      void DeleteColliderMap(const int tileMapID);
  };

}

#endif