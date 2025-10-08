#ifndef TILES_H
#define TILES_H

#include "Sprocket.h"

namespace Sprocket {

    class Tiles : public Entity {

        // TODO for some reason this logic has to be in the constructor instead of the start call 
        // which may be okay, but should be investigated
    public:

        Tiles() {
            AddComponent<TileMapComponent>();
        }

        void Start() override {
            GetComponent<TileMapComponent>()->SetColliderMapPath("tilemaps/ColliderTileMap.txt");
            GetComponent<TileMapComponent>()->SetQuadRendererMapPath("tilemaps/SpriteTileMap.txt");

            auto sprites = GetSpritesFromSpritesheet("textures/NewDirtTopDownTiles.png", 960, 960, 96, 96);

            // 0 Full Grass
            GetComponent<TileMapComponent>()->SetQuadRendererData('0', sprites[55]);
            // 1 Full Dirt
            GetComponent<TileMapComponent>()->SetQuadRendererData('1', sprites[83]);
            // 2 Grass Top Dirt Bottom
            GetComponent<TileMapComponent>()->SetQuadRendererData('2', sprites[45]);
            // 3 Grass Left Dirt Right
            GetComponent<TileMapComponent>()->SetQuadRendererData('3', sprites[56]);
            // 4 Grass Right Dirt Left
            GetComponent<TileMapComponent>()->SetQuadRendererData('4', sprites[54]);
            // 5 Grass Bottom Dirt Top
            GetComponent<TileMapComponent>()->SetQuadRendererData('5', sprites[65]);
            // 6 Dirt Bottom Right
            GetComponent<TileMapComponent>()->SetQuadRendererData('6', sprites[21]);
            // 7 Dirt Bottom Left
            GetComponent<TileMapComponent>()->SetQuadRendererData('7', sprites[22]);
            // 8 Dirt Top Left
            GetComponent<TileMapComponent>()->SetQuadRendererData('8', sprites[12]);
            // 9 Dirt Top Right
            GetComponent<TileMapComponent>()->SetQuadRendererData('9', sprites[11]);
            // a Grass Bottom Right
            GetComponent<TileMapComponent>()->SetQuadRendererData('a', sprites[81]);
            // b Grass Bottom Left
            GetComponent<TileMapComponent>()->SetQuadRendererData('b', sprites[82]);
            // c Grass Top Left
            GetComponent<TileMapComponent>()->SetQuadRendererData('c', sprites[72]);
            // d Grass Top Right
            GetComponent<TileMapComponent>()->SetQuadRendererData('d', sprites[71]);
        }

    };

}

#endif