#include "ECS/TileMap.h"

#include <fstream>
#include <iostream>

namespace Sprocket {

  TileMap::TileMap(QuadRenderer* quadRenderer, Physics* physics) : m_QuadRenderer(quadRenderer), m_Physics(physics) {
    m_FreeIDs.push(0);
  }

  void TileMap::RegisterTileMap(TransformComponent transform, TileMapComponent& tcomp) {

    GenerateTileMapID(tcomp);

    IntializeTileMapStorage(tcomp);

    RegisterSpriteMap(transform, tcomp);

    RegisterColliderMap(transform, tcomp);

  }

  void TileMap::DeleteTileMap(const int tilemapID) {

    // If the given tilemapID is not in use
    if(!m_QuadRendererIDs.count(tilemapID) && !m_PhysicsPointers.count(tilemapID)) {
      throw std::invalid_argument("The given tilemapID does not correspond to a valid TileMapComponent.");
    }

    DeleteSpriteMap(tilemapID);
    DeleteColliderMap(tilemapID);

    // Since this ID is now free, add it to the queue of free IDs
    m_FreeIDs.push(tilemapID);

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////// HELPER FUNCTIONS ///////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  void TileMap::GenerateTileMapID(TileMapComponent& tilemap) {

    // If there is only one index, that all indicies before it are filled and all after a free, so 
    // add the next free index.
    if(m_FreeIDs.size() == 1) {
      m_FreeIDs.push(m_FreeIDs.top() + 1);
    }

    // Assign the tilemapID as the next free ID
    tilemap.tilemapID = m_FreeIDs.top();

    // Remove the newly assigned ID from the list of freeIDs
    m_FreeIDs.pop();

  }

  void TileMap::IntializeTileMapStorage(TileMapComponent& tilemap) {
    // Create a new vector of IDs for quadrenderers for this tilemap at given tilemapID
    m_QuadRendererIDs.insert({tilemap.tilemapID, std::vector<int>()});

    // Create a new entry in the map with a map for all colliders in this tilemap
    m_PhysicsPointers.insert({
        (unsigned int)m_PhysicsPointers.size(), 
        std::map<
            unsigned int, 
            std::tuple<TransformComponent*,
            TransformComponent*,PhysicsComponent*,
            ColliderComponent*>>()
    });
  }

  void TileMap::RegisterSpriteMap(TransformComponent transform, TileMapComponent& tcomp) {

    std::ifstream spriteFile(tcomp.spriteMapPath);
    std::string line;

    std::getline(spriteFile, line);

    // Get the indicies of the origin tile
    int dash = line.find('-');
    int xOrigin = std::stoi(line.substr(0,dash));
    int yOrigin = std::stoi(line.substr(dash+1)); 

    

    int row = 0;
    int col = 0;

    while(std::getline(spriteFile,line)) {
      
      // Iterate through the string
      while(col < line.size()) {

        char c = line.at(col);

        // Nothing needs to be rendered if the char at the current position is an x
        if(c != 'x')  {

          // TODO validate the character

          TransformComponent tileTComp = transform;
          tileTComp.position.x += (col - xOrigin) * tcomp.tileSize;
          tileTComp.position.y -= (row - yOrigin) * tcomp.tileSize;

          QuadRendererComponent tileQComp;
          tileQComp.size = tcomp.tileSize;
          
          
          // Translate the char to a string and gets its integer value to get the corrsponding
          // texture path
          std::string cAsInt;
          cAsInt.push_back(c);
          tileQComp.sprite = tcomp.sprites.at(std::stoi(cAsInt));
          //tileQComp.sprite.texturePath =  tcomp.texturePaths.at(std::stoi(cAsInt));

          // Render and update the quad
          m_QuadRenderer->RenderNewQuad(tileTComp,tileQComp);
          m_QuadRenderer->UpdateQuad(tileQComp);

          // Store this ID of the QuadRender in the corresponding vector
          m_QuadRendererIDs.at(tcomp.tilemapID).push_back(tileQComp.quadID);

        }

        col++;
      }
      // Reset the column count
      col = 0;
    
      row++;
    }

    spriteFile.close();

  }

  // TODO optimize this so that multiple box colliders that are right next together are merged into a single collider
  void TileMap::RegisterColliderMap(TransformComponent transform, TileMapComponent& tcomp) {

    std::ifstream colliderFile(tcomp.colliderMapPath);
    std::string line;

    std::getline(colliderFile, line);

    // Get the indicies of the origin tile
    int dash = line.find('-');
    int xOrigin = std::stoi(line.substr(0,dash));
    int yOrigin = std::stoi(line.substr(dash+1));

    int row = 0;
    int col = 0;

    while(std::getline(colliderFile,line)) {
      
      // Iterate through the string
      while(col < line.size()) {  

        char c = line.at(col);

        // Nothing needs to be rendered if the char at the current position is an x
        if(c == '0')  {

          TransformComponent* tileTComp = new TransformComponent(transform);
          tileTComp->position.x += (col - xOrigin) * tcomp.tileSize;
          tileTComp->position.y -= (row - yOrigin) * tcomp.tileSize;

          PhysicsComponent* pcomp = new PhysicsComponent();
          //pcomp->isDynamic = false;

          BoxColliderComponent* bcomp = new BoxColliderComponent();
          bcomp->height = tcomp.tileSize;
          bcomp->width = tcomp.tileSize;
          

          // Insert a new tuple of pointers into the map corresponding to this tilemap
          m_PhysicsPointers.at(tcomp.tilemapID).insert({(unsigned int)m_PhysicsPointers.at(tcomp.tilemapID).size(), {tileTComp, new TransformComponent(), pcomp, bcomp}});

          // Register a new physics object based on the information that was just inserted into the map
          m_Physics->RegisterNewPhysicsObject(
            *std::get<0>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1)), 
            *std::get<1>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1)), 
            *std::get<2>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1)), 
            *std::get<3>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1)));
        }

        col++;
      }
      // Reset the column count
      col = 0;
    
      row++;
    }

    colliderFile.close();
    
  }

  void TileMap::DeleteSpriteMap(const int tilemapID) {

    // For each id in the vector on IDs, delete the quad
    for(auto i : m_QuadRendererIDs.at(tilemapID)) { 
      m_QuadRenderer->DeleteQuad(i);

    }

    // Remove the vector from the map
    m_QuadRendererIDs.extract(tilemapID);

  }

  void TileMap::DeleteColliderMap(const int tileMapID) {
    
    // For each tuple of pointers in the map, use the physicsID to delete the collider
    for(auto it = m_PhysicsPointers.at(tileMapID).begin(); it != m_PhysicsPointers.at(tileMapID).end(); it++) {
      m_Physics->DeletePhysicsObject(std::get<2>(it->second)->phyiscsID);
    }

    // Remove the map from the map
    m_PhysicsPointers.extract(tileMapID);

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////


}