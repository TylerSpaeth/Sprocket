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
            // 0 Full Grass
            Sprite fullGrass = { "textures/NewDirtTopDownTiles.png", { 575 / 959.0f,575 / 959.0f,480 / 959.0f,480 / 959.0f }, { 575 / 959.0f,480 / 959.0f,480 / 959.0f,575 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('0', fullGrass);
            // 1 Full Dirt
            Sprite fullDirt = { "textures/NewDirtTopDownTiles.png", { 383 / 959.0f,383 / 959.0f,288 / 959.0f,288 / 959.0f }, { 863 / 959.0f,768 / 959.0f,768 / 959.0f,863 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('1', fullDirt);
            // 2 Grass Top Dirt Bottom
            Sprite grassTopDirtBottom = { "textures/NewDirtTopDownTiles.png", { 575 / 959.0f,575 / 959.0f,480 / 959.0f,480 / 959.0f }, { 479 / 959.0f,384 / 959.0f,384 / 959.0f,479 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('2', grassTopDirtBottom);
            // 3 Grass Left Dirt Right
            Sprite grassLeftDirtRight = { "textures/NewDirtTopDownTiles.png", { 671 / 959.0f,671 / 959.0f,576 / 959.0f,576 / 959.0f }, { 575 / 959.0f,480 / 959.0f,480 / 959.0f,575 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('3', grassLeftDirtRight);
            // 4 Grass Right Dirt Left
            Sprite grassRightDirtLeft = { "textures/NewDirtTopDownTiles.png", { 479 / 959.0f,479 / 959.0f,384 / 959.0f,384 / 959.0f }, { 575 / 959.0f,480 / 959.0f,480 / 959.0f,575 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('4', grassRightDirtLeft);
            // 5 Grass Bottom Dirt Top
            Sprite grassBottomDirtTop = { "textures/NewDirtTopDownTiles.png", { 575 / 959.0f,575 / 959.0f,480 / 959.0f,480 / 959.0f }, { 671 / 959.0f,576 / 959.0f,576 / 959.0f,671 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('5', grassBottomDirtTop);
            // 6 Dirt Bottom Right
            Sprite dirtBottomRight = { "textures/NewDirtTopDownTiles.png", { 191 / 959.0f,191 / 959.0f,96 / 959.0f,96 / 959.0f }, { 287 / 959.0f,192 / 959.0f,192 / 959.0f,287 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('6', dirtBottomRight);
            // 7 Dirt Bottom Left
            Sprite dirtBottomLeft = { "textures/NewDirtTopDownTiles.png", { 287 / 959.0f,287 / 959.0f,192 / 959.0f,192 / 959.0f }, { 287 / 959.0f,192 / 959.0f,192 / 959.0f,287 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('7', dirtBottomLeft);
            // 8 Dirt Top Left
            Sprite dirtTopLeft = { "textures/NewDirtTopDownTiles.png", { 287 / 959.0f,287 / 959.0f,192 / 959.0f,192 / 959.0f }, { 191 / 959.0f,96 / 959.0f,96 / 959.0f,191 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('8', dirtTopLeft);
            // 9 Dirt Top Right
            Sprite dirtTopRight = { "textures/NewDirtTopDownTiles.png", { 191 / 959.0f,191 / 959.0f,96 / 959.0f,96 / 959.0f }, { 191 / 959.0f,96 / 959.0f,96 / 959.0f,191 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('9', dirtTopRight);
            // a Grass Bottom Right
            Sprite grassBottomRight = { "textures/NewDirtTopDownTiles.png", { 191 / 959.0f,191 / 959.0f,96 / 959.0f,96 / 959.0f }, { 863 / 959.0f,768 / 959.0f,768 / 959.0f,863 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('a', grassBottomRight);
            // b Grass Bottom Left
            Sprite grassBottomLeft = { "textures/NewDirtTopDownTiles.png", { 287 / 959.0f,287 / 959.0f,192 / 959.0f,192 / 959.0f }, { 863 / 959.0f,768 / 959.0f,768 / 959.0f,863 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('b', grassBottomLeft);
            // c Grass Top Left
            Sprite grassTopLeft = { "textures/NewDirtTopDownTiles.png", { 287 / 959.0f,287 / 959.0f,192 / 959.0f,192 / 959.0f }, { 767 / 959.0f,672 / 959.0f,672 / 959.0f,767 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('c', grassTopLeft);
            // d Grass Top Right
            Sprite grassTopRight = { "textures/NewDirtTopDownTiles.png", { 191 / 959.0f,191 / 959.0f,96 / 959.0f,96 / 959.0f }, { 767 / 959.0f,672 / 959.0f,672 / 959.0f,767 / 959.0f } };
            GetComponent<TileMapComponent>()->SetQuadRendererData('d', grassTopRight);
        }

    };

}

#endif