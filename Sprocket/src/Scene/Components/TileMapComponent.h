#ifndef TILEMAPCOMPONENT_H
#define TILEMAPCOMPONENT_H

#include "Component.h"
#include "Subcomponents/Sprite.h"

#include "ThirdParty/glm/glm.hpp"

#include "Core/Sprocket.pch"

namespace Sprocket {

    // FIXME for some reason rotating the tilemap has a major negative impact on performance.
    // Not sure what is causing this

    /// @brief Component for rendered and collider tilemaps.
    class SPROCKET_API TileMapComponent : public EventDrivenComponent {

        friend class Entity;

    private:
    
        // Global transform values for the entire tilemap
    
        glm::vec3 m_Position;
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;

        
        std::string m_QuadRendererMapPath;
        std::string m_ColliderMapPath;

        // The ids of all the colliders in the physics system
        std::vector<unsigned int> m_PhysicsIDs;

        // QuadRenderer maps have a maximum number of unique tiles that are defined by the printable 
        // ascii characters. The ~ character is used to denote no sprite and can not be mapped.
        // The arrays are based on the first printable ascii character are sized to be able to map
        // all ascii characters except for ~.

        static constexpr char EMPTY_PLACEHOLDER_CHAR = '~';
        static constexpr char FIRST_PRINTABLE_ASCII = '!';
        static constexpr unsigned int MAX_UNIQUE_TILES = 94;
        std::array<std::vector<unsigned int>, MAX_UNIQUE_TILES> m_QuadRendererIDs;
        std::array<Sprite, MAX_UNIQUE_TILES> m_Sprites;

    public:

        ~TileMapComponent();
        
        /// @brief Sets the QuadRenderer map path. This is a text file that defines the layout of the
        /// sprites in the tilemap. Each character in the text file corresponds to a unique sprite.
        /// @param quadRendererMapPath - The path to the QuadRenderer map file.
        void SetQuadRendererMapPath(const std::string quadRendererMapPath);

        /// @brief Sets the Collider map path. This is a text file that defines the layout of the
        /// colliders in the tilemap. 0s represent a collider while everything else is empty space.
        /// @param colliderMapPath - The path to the Collider map file. 
        void SetColliderMapPath(const std::string colliderMapPath);

        /// @brief Sets the sprite of the QuadRenderer at the given index.
        /// @param index - The index of the QuadRenderer to set the texture and UV coordinates of.
        /// @param sprite - The sprite to assign to the QuaderRenderer at index.
        const bool SetQuadRendererData(const char index, const Sprite& sprite);

    private:

        /// @brief Registers the TileMap with the given transform values.
        /// @param position - The position of the tile map.
        /// @param rotation - The rotation of the time map.
        /// @param scale - The scale of the tile map.
        void RegisterTileMap(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        /// @brief Registers the QuadRenderer map.
        void RegisterQuadRendererMap();

        /// @brief Registers the Collider map.
        void RegisterColliderMap();

        /// @brief Updates the transform of the tile map.
        /// @param position - The position of the tile map.
        /// @param rotation - The rotation of the tile map.
        /// @param scale - The scale of the tile map.
        void UpdateTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);

        /// @brief Deletes the tile map, removing it from the relevant systems.
        void DeleteTileMap();

        /// @brief Delete the QuadRenderer portion of the tile map from the relevant systems.
        void DeleteQuadRendererMap();

        /// @brief Delete the Collider portion of the tile map from the relevant systems.
        void DeleteColliderMap();

    };

}

#endif