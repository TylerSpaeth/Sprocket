#ifndef TILES_H
#define TILES_H

#include "Sprocket.h"

namespace Sprocket {

  class Tiles : public Entity {

    // TODO for some reason this logic has to be in the constructor instead of the start call which 
    // may be okay, but should be investigated
    public:

      Tiles() {
        this->AddComponent<TileMapComponent>();
        this->GetComponent<TileMapComponent>()->SetColliderMapPath("../res/tilemaps/ColliderTileMap.txt");
        this->GetComponent<TileMapComponent>()->SetQuadRendererMapPath("../res/tilemaps/SpriteTileMap.txt");
        glm::vec4 color1 = {1,1,1,1};
        glm::vec4 color = {1,0,1,1};
        this->GetComponent<TileMapComponent>()->SetQuadRendererData(0, color1);
        this->GetComponent<TileMapComponent>()->SetQuadRendererData(1, color);
      }

      void Start() override {
        // this->AddComponent<TileMapComponent>();
        // this->GetComponent<TileMapComponent>()->SetColliderMapPath("../res/tilemaps/ColliderTileMap.txt");
        // this->GetComponent<TileMapComponent>()->SetQuadRendererMapPath("../res/tilemaps/SpriteTileMap.txt");
        // glm::vec4 color1 = {1,1,1,1};
        // glm::vec4 color = {1,0,1,1};
        // this->GetComponent<TileMapComponent>()->SetQuadRendererData(0, color1);
        // this->GetComponent<TileMapComponent>()->SetQuadRendererData(1, color);
      }
 
  };
 
}

#endif