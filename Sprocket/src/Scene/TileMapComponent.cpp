#include "TileMapComponent.h"

#include "Events/PhysicsEvent.h"
#include "Events/RenderEvent.h"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <fstream>
#include <iostream>

namespace Sprocket {

  void TileMapComponent::RegisterTileMap(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
    m_Position = position;
    m_Rotation = rotation;
    m_Scale = scale;
    RegisterQuadRendererMap();
    RegisterColliderMap();
  }

  void TileMapComponent::RegisterQuadRendererMap() {
    
    std::ifstream quadRendererMapFile(m_QuadRendererMapPath);
    std::string line;

    std::getline(quadRendererMapFile, line);

    if(line.empty()) return;

    // Get the indices of the origin tile
    int dash = line.find('-');
    int xOrigin = std::stoi(line.substr(0,dash));
    int yOrigin = std::stoi(line.substr(dash+1));

    int row = 0;
    int col = 0;

    while(std::getline(quadRendererMapFile, line)) {

      // Iterate through the string
      while(col < line.size()) {

        char c = line.at(col);

        // If the character at c is a number between 0 and 9, then something will be rendered, 
        // otherwise, nothing is rendered
        if(c >= '0' && c <= '9')  {

          glm::vec3 tilePosition = m_Position;
          tilePosition.x += (col-xOrigin) * m_Scale.x;
          tilePosition.y -= (row-yOrigin) * m_Scale.y;

          // Translate the char to a string and gets its integer value to get the corrsponding
          // texture path
          std::string cAsInt;
          cAsInt.push_back(c);

          QuadRendererStruct quadRendererStruct = m_QuadRenderers.at(std::stoi(cAsInt));
          
          RenderNewEvent* renderNewEvent = new RenderNewEvent();
          m_EventCallback(*renderNewEvent);
          unsigned int quadID = renderNewEvent->GetQuadID();
          m_QuadRendererIDs.at(std::stoi(cAsInt)).push_back(quadID);
          free(renderNewEvent);

          RenderUpdateEvent* renderUpdateEventQuad = new RenderUpdateEvent(RenderUpdateType::QUAD, quadID);
          if(!quadRendererStruct.m_TexturePath.empty()) {
            renderUpdateEventQuad->m_TexturePath = quadRendererStruct.m_TexturePath;
            renderUpdateEventQuad->m_TexXCoords = quadRendererStruct.m_TextureXUVCoords;
            renderUpdateEventQuad->m_TexYCoords = quadRendererStruct.m_TextureYUVCoords;
          }
          else {
            renderUpdateEventQuad->m_QuadColor = quadRendererStruct.m_QuadColor;
          }
          m_EventCallback(*renderUpdateEventQuad);
          free(renderUpdateEventQuad);

          RenderUpdateEvent* renderUpdateEventModel = new RenderUpdateEvent(RenderUpdateType::MODEL_MATRIX, quadID);
          // Translating twice is because the matrix needs to be rotated relative to the center of
          // the tile map
          renderUpdateEventModel->m_Matrix = glm::translate(glm::mat4(1), m_Position)
                                           * glm::rotate(glm::mat4(1), glm::radians(-m_Rotation.z), glm::vec3(0,0,1))
                                           * glm::translate(glm::mat4(1), tilePosition)
                                           * glm::scale(glm::mat4(1), m_Scale);

          m_EventCallback(*renderUpdateEventModel);
          free(renderUpdateEventModel);

        }

        col++;

      }

      col = 0;
      row++;

    }

    quadRendererMapFile.close();

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

  void TileMapComponent::RegisterColliderMap() {

    std::ifstream colliderMapFile(m_ColliderMapPath);
    std::string line;

    std::getline(colliderMapFile, line);

    if(line.empty()) return;

    // Get the indicies of the origin tile
    int dash = line.find('-');
    int xOrigin = std::stoi(line.substr(0,dash));
    int yOrigin = std::stoi(line.substr(dash+1));

    // A 2d vector that is identical to the given file, but instead a true value is used to 
    // represent the fact that a collider needs to be placed there
    std::vector<std::vector<bool>> boolMap;
    LoadBoolMap(boolMap,colliderMapFile);
    colliderMapFile.close();

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
          glm::vec3 tilePosition = m_Position;
          tilePosition.x += ((xEnd-xOrigin)+(xStart-xOrigin))/2.0f * m_Scale.x;
          tilePosition.y -= ((yEnd-yOrigin)+(yStart-yOrigin))/2.0f  * m_Scale.y;

          // Calculate the position and rotation around the center of the tilemap
          // Translating twice is because the matrix needs to be rotated relative to the center of
          // the tile map
          glm::mat4 matrix = glm::translate(glm::mat4(1), m_Position)
                                           * glm::rotate(glm::mat4(1), glm::radians(-m_Rotation.z), glm::vec3(0,0,1))
                                           * glm::translate(glm::mat4(1), tilePosition)
                                           * glm::scale(glm::mat4(1), m_Scale);  
          glm::vec3 position(matrix[3]);
          float angleRadians = atan2(matrix[1][0], matrix[0][0]);  // model[col][row]
          float zRotation = glm::degrees(angleRadians);

          PhysicsNewEvent* event = new PhysicsNewEvent(position, {(xEnd+1-xStart) * m_Scale.x, (yEnd+1-yStart) * m_Scale.y}, zRotation);
          m_EventCallback(*event);
          m_PhysicsIDs.push_back(event->GetPhysicsID());
          free(event);
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

  void TileMapComponent::UpdateTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {

    m_Position = position;
    m_Rotation = rotation;
    m_Scale = scale;

    // TODO optimize this behavior because this is not ideal
    DeleteTileMap();
    RegisterColliderMap();
    RegisterQuadRendererMap();
  }

  void TileMapComponent::DeleteTileMap() {
    DeleteColliderMap();
    DeleteQuadRendererMap();
  }

  void TileMapComponent::DeleteQuadRendererMap() {
    // Remove the quad renderers from the rendering system
    for(auto& vector : m_QuadRendererIDs) {

      for(auto quadRendererID : vector) {

        RenderDeleteEvent* event = new RenderDeleteEvent(quadRendererID);

        m_EventCallback(*event);

        free(event);
      }

      vector.clear();

    }
  }

  void TileMapComponent::DeleteColliderMap() {
    // Remove the colliders from the physics system
    for(auto& physicsID : m_PhysicsIDs) {

      PhysicsDeleteEvent* event = new PhysicsDeleteEvent(physicsID);

      m_EventCallback(*event);

      free(event);

    }
    m_PhysicsIDs.clear();
  }

  void TileMapComponent::SetQuadRendererMapPath(const std::string quadRendererMapPath) {

    m_QuadRendererMapPath = quadRendererMapPath;

    if(m_EventCallback == nullptr) return;

    DeleteQuadRendererMap();
    RegisterQuadRendererMap();
  }

  void TileMapComponent::SetColliderMapPath(const std::string colliderMapPath) {
    
    m_ColliderMapPath = colliderMapPath;

    if(m_EventCallback == nullptr) return;

    DeleteColliderMap();
    RegisterColliderMap();
  }

  void TileMapComponent::SetQuadRendererData(const unsigned int index, const glm::vec4 quadColor) {
    if(index > 9) {
      return;
    }
    m_QuadRenderers.at(index).m_QuadColor = quadColor;
    if(m_EventCallback != nullptr) {
      for(auto quadRendererID : m_QuadRendererIDs.at(index)) {
        RenderUpdateEvent* event = new RenderUpdateEvent(RenderUpdateType::QUAD, quadRendererID);
        event->m_QuadColor = quadColor;
        m_EventCallback(*event);
        free(event);
      }
    }
  }

  void TileMapComponent::SetQuadRendererData(const unsigned int index, const std::string texturePath) {
    if(index > 9) {
      return;
    }
    QuadRendererStruct& qrs = m_QuadRenderers.at(index);
    qrs.m_TexturePath = texturePath;
    if(m_EventCallback != nullptr) {
      for(auto quadRendererID : m_QuadRendererIDs.at(index)) {
        RenderUpdateEvent* event = new RenderUpdateEvent(RenderUpdateType::QUAD, quadRendererID);
        event->m_TexturePath = qrs.m_TexturePath;
        event->m_TexXCoords = qrs.m_TextureXUVCoords;
        event->m_TexYCoords = qrs.m_TextureYUVCoords;
        m_EventCallback(*event);
        free(event);
      }
    }
  }

  void TileMapComponent::SetQuadRendererData(const unsigned int index, const std::string texturePath, const glm::vec4 textureXUVCoords, const glm::vec4 textureYUVCoords) {
    if(index > 9) {
      return;
    }
    QuadRendererStruct& qrs = m_QuadRenderers.at(index);
    qrs.m_TexturePath = texturePath;
    qrs.m_TextureXUVCoords = textureXUVCoords;
    qrs.m_TextureYUVCoords = textureYUVCoords;

    if(m_EventCallback != nullptr) {
      for(auto quadRendererID : m_QuadRendererIDs.at(index)) {
        RenderUpdateEvent* event = new RenderUpdateEvent(RenderUpdateType::QUAD, quadRendererID);
        event->m_TexturePath = texturePath;
        event->m_TexXCoords = qrs.m_TextureXUVCoords;
        event->m_TexYCoords = qrs.m_TextureYUVCoords;
        m_EventCallback(*event);
        free(event);
      }
    }
  }

}