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
        // 0 Full Grass
        GetComponent<TileMapComponent>()->SetQuadRendererData('0', "../res/textures/NewDirtTopDownTiles.png", {575/959.0f,575/959.0f,480/959.0f,480/959.0f},{575/959.0f,480/959.0f,480/959.0f,575/959.0f});
        // 1 Full Dirt
        GetComponent<TileMapComponent>()->SetQuadRendererData('1', "../res/textures/NewDirtTopDownTiles.png", {383/959.0f,383/959.0f,288/959.0f,288/959.0f},{863/959.0f,768/959.0f,768/959.0f,863/959.0f});
        // 2 Grass Top Dirt Bottom
        GetComponent<TileMapComponent>()->SetQuadRendererData('2', "../res/textures/NewDirtTopDownTiles.png", {575/959.0f,575/959.0f,480/959.0f,480/959.0f},{479/959.0f,384/959.0f,384/959.0f,479/959.0f});
        // 3 Grass Left Dirt Right
        GetComponent<TileMapComponent>()->SetQuadRendererData('3', "../res/textures/NewDirtTopDownTiles.png", {671/959.0f,671/959.0f,576/959.0f,576/959.0f},{575/959.0f,480/959.0f,480/959.0f,575/959.0f});
        // 4 Grass Right Dirt Left
        GetComponent<TileMapComponent>()->SetQuadRendererData('4', "../res/textures/NewDirtTopDownTiles.png", {479/959.0f,479/959.0f,384/959.0f,384/959.0f},{575/959.0f,480/959.0f,480/959.0f,575/959.0f});
        // 5 Grass Bottom Dirt Top
        GetComponent<TileMapComponent>()->SetQuadRendererData('5', "../res/textures/NewDirtTopDownTiles.png", {575/959.0f,575/959.0f,480/959.0f,480/959.0f},{671/959.0f,576/959.0f,576/959.0f,671/959.0f});
        // 6 Dirt Bottom Right
        GetComponent<TileMapComponent>()->SetQuadRendererData('6', "../res/textures/NewDirtTopDownTiles.png", {191/959.0f,191/959.0f,96/959.0f,96/959.0f},{287/959.0f,192/959.0f,192/959.0f,287/959.0f});
        // 7 Dirt Bottom Left
        GetComponent<TileMapComponent>()->SetQuadRendererData('7', "../res/textures/NewDirtTopDownTiles.png", {287/959.0f,287/959.0f,192/959.0f,192/959.0f},{287/959.0f,192/959.0f,192/959.0f,287/959.0f});
        // 8 Dirt Top Left
        GetComponent<TileMapComponent>()->SetQuadRendererData('8', "../res/textures/NewDirtTopDownTiles.png", {287/959.0f,287/959.0f,192/959.0f,192/959.0f},{191/959.0f,96/959.0f,96/959.0f,191/959.0f});
        // 9 Dirt Top Right
        GetComponent<TileMapComponent>()->SetQuadRendererData('9', "../res/textures/NewDirtTopDownTiles.png", {191/959.0f,191/959.0f,96/959.0f,96/959.0f},{191/959.0f,96/959.0f,96/959.0f,191/959.0f});
        // a Grass Bottom Right
        GetComponent<TileMapComponent>()->SetQuadRendererData('a', "../res/textures/NewDirtTopDownTiles.png", {191/959.0f,191/959.0f,96/959.0f,96/959.0f},{863/959.0f,768/959.0f,768/959.0f,863/959.0f});
        // b Grass Bottom Left
        GetComponent<TileMapComponent>()->SetQuadRendererData('b', "../res/textures/NewDirtTopDownTiles.png", {287/959.0f,287/959.0f,192/959.0f,192/959.0f},{863/959.0f,768/959.0f,768/959.0f,863/959.0f});
        // c Grass Top Left
        GetComponent<TileMapComponent>()->SetQuadRendererData('c', "../res/textures/NewDirtTopDownTiles.png", {287/959.0f,287/959.0f,192/959.0f,192/959.0f},{767/959.0f,672/959.0f,672/959.0f,767/959.0f});
        // d Grass Top Right
        GetComponent<TileMapComponent>()->SetQuadRendererData('d', "../res/textures/NewDirtTopDownTiles.png", {191/959.0f,191/959.0f,96/959.0f,96/959.0f},{767/959.0f,672/959.0f,672/959.0f,767/959.0f});
        
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