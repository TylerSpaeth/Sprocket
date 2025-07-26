#ifndef TILES_H
#define TILES_H

#include "Sprocket.h"

namespace Sprocket {

  class Tiles : public Entity {

    // TODO for some reason this logic has to be in the constructor instead of the start call which 
    // may be okay, but should be investigated
    public:

      Tiles() {
        AddComponent<TileMapComponent>();
        GetComponent<TileMapComponent>()->SetColliderMapPath("../res/tilemaps/ColliderTileMap.txt");
        GetComponent<TileMapComponent>()->SetQuadRendererMapPath("../res/tilemaps/SpriteTileMap.txt");
        glm::vec4 color1 = {1,1,1,1};
        glm::vec4 color = {1,0,1,1};
        GetComponent<TileMapComponent>()->SetQuadRendererData(0, color1);
        GetComponent<TileMapComponent>()->SetQuadRendererData(1, color);
      }

      void Start() override {
        // AddComponent<TileMapComponent>();
        // GetComponent<TileMapComponent>()->SetColliderMapPath("../res/tilemaps/ColliderTileMap.txt");
        // GetComponent<TileMapComponent>()->SetQuadRendererMapPath("../res/tilemaps/SpriteTileMap.txt");
        // glm::vec4 color1 = {1,1,1,1};
        // glm::vec4 color = {1,0,1,1};
        // GetComponent<TileMapComponent>()->SetQuadRendererData(0, color1);
        // GetComponent<TileMapComponent>()->SetQuadRendererData(1, color);
      }
 
  };
 
}

#endif