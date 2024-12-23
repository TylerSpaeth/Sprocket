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

        // If the character at c is a number between 0 and 9, then something will be rendered, 
        // otherwise, nothing is rendered
        if(c >= '0' && c <= '9')  {

          TransformComponent tileTComp = transform;
          tileTComp.position.x += (col - xOrigin) * tcomp.tileSize;
          tileTComp.position.y -= (row - yOrigin) * tcomp.tileSize;

          QuadRendererComponent tileQComp(tcomp.tileSize);
          
          
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

  // Loads the given 2d vector with bool value of true if there is a 0 at the corresponding row and
  // col of the colliderFile or false if there is an x or anything else.
  void LoadBoolMap(std::vector<std::vector<bool>>& map, std::ifstream& colliderFile) {
    std::string line;

    int row = 0;
    int col = 0;

    while(std::getline(colliderFile,line)) {
      map.push_back(std::vector<bool>());

      // Iterate through the string
      while(col < line.size()) {  

        char c = line.at(col);

        // Only create a box collider if the character is 0
        if(c == '0')  {
          map.at(row).push_back(true);
        }
        else {
          map.at(row).push_back(false);
        }

        col++;
      }
      // Reset the column count
      col = 0;
    
      row++;
    }

  }

  void TileMap::RegisterColliderMap(TransformComponent transform, TileMapComponent& tcomp) {

    std::ifstream colliderFile(tcomp.colliderMapPath);
    std::string line;

    std::getline(colliderFile, line);

    // Get the indicies of the origin tile
    int dash = line.find('-');
    int xOrigin = std::stoi(line.substr(0,dash));
    int yOrigin = std::stoi(line.substr(dash+1));

    // A 2d vector that is identical to the given file, but instead a true value is used to 
    // represent the fact that a collider needs to be placed there
    std::vector<std::vector<bool>> boolMap;
    LoadBoolMap(boolMap,colliderFile);
    colliderFile.close();

    // Now create colliders based on the bool map 

    int row = 0;
    int col = 0;

    // This loop searches for the top-left most top-left true value in the boolMap and then finds
    // finds adjacent colliders that can be combined into it. A priority is given to horizontal 
    // joining over vertical. All tiles that have had a collider placed on them are set to false 
    // and then the pointer are reset to zero for a new pass.
    while(row < boolMap.size()) {

      bool cur = boolMap.at(row).at(col);

      if(cur) {

        int xStart = col;
        int xEnd = col;
        int yStart = row;
        int yEnd = row;

        // Move left to right across the row until a non collider is found or the end is reached
        while(cur != false) {
          xEnd = col;
          col++;
          if(col == boolMap.at(row).size()) break;
          cur = boolMap.at(row).at(col);
        }

        col = xStart;
        row++;
        // Check the xStart-xEnd region on every row below until one of them does not contain just 
        // colliders
        while(row != boolMap.size()) {

          // If the current row is shorter than the xEnd of the other rows, then we need to break.
          if(boolMap.at(row).size()-1 < xEnd) break;

          // A row is valid unless any of the elements in the row between xStart and xEnd are false
          bool validRow = true;
          for(int i = xStart; i <= xEnd; i++) {
            if(boolMap.at(row).at(i) == false) {
              validRow = false;
            }
          }

          // If the current row is not valid, then no need to do anything else
          if(!validRow) break;

          // Otherwise update the yEnd to match the row that was just checked, and move the row 
          // pointer to the next row.
          row++;
          yEnd++;

        }

        // Create the collider
        {
          TransformComponent* tileTComp = new TransformComponent(transform);
          tileTComp->position.x += ((xEnd-xOrigin)+(xStart-xOrigin))/2.0f * tcomp.tileSize;
          tileTComp->position.y -= ((yEnd-yOrigin)+(yStart-yOrigin))/2.0f * tcomp.tileSize;

          PhysicsComponent* pcomp = new PhysicsComponent();
          //pcomp->isDynamic = false;

          BoxColliderComponent* bcomp = new BoxColliderComponent();
          bcomp->height = tcomp.tileSize * (yEnd+1-yStart);
          bcomp->width = tcomp.tileSize * (xEnd+1-xStart);

          // Insert a new tuple of pointers into the map corresponding to this tilemap
          m_PhysicsPointers.at(tcomp.tilemapID).insert({(unsigned int)m_PhysicsPointers.at(tcomp.tilemapID).size(), {tileTComp, new TransformComponent(), pcomp, bcomp}});

          auto t1 = *std::get<0>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1));
          auto t2 = *std::get<1>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1));

          t1.position += t2.position;
          t1.rotation += t2.rotation;
          t1.scale *= t2.scale;

          // Register a new physics object based on the information that was just inserted into the map
          m_Physics->RegisterNewPhysicsObject(
            t1,
            *std::get<2>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1)), 
            *std::get<3>(m_PhysicsPointers.at(tcomp.tilemapID).at(m_PhysicsPointers.at(tcomp.tilemapID).size()-1)));
        }

        // Set the boolMap values associated with this new collider to false
        {
          for(int i = xStart; i <= xEnd; i++) {
            for(int j = yStart; j <= yEnd; j++) {
              boolMap.at(j).at(i) = false;
            } 
          }
        }

        // Reset the row and col for the next pass
        row = 0;
        col = 0;

      }

      // If the current tile does not need to be a collider, then update the pointers.
      else {
        if(col == boolMap.at(row).size()-1) {
          row++;
          col = 0;
        }
        else {
          col++;
        }
      }

    }
    
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
      m_Physics->DeletePhysicsObject(std::get<2>(it->second)->physicsID);
    }

    // Remove the map from the map
    m_PhysicsPointers.extract(tileMapID);

  }

  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////


}