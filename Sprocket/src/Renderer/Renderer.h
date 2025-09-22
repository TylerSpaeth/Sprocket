#ifndef RENDERER_H
#define RENDERER_H

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Events/Event.h"
#include "Core/Sprocket.pch"
#include "Font.h"

#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

namespace Sprocket {

    struct Vertex {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TextureCoords;
        float TextureID;
        bool operator==(const Vertex& other) const {
            return Position == other.Position &&
                Color == other.Color &&
                TextureCoords == other.TextureCoords &&
                TextureID == other.TextureID;
        }
    };

    // TODO the shader system for the renderer needs rework

    /// @brief This handles all of the rendering functionality for a Sprocket
    /// application using OpenGL. The sorting order of quads is determined by z position.
    /// High z positions are rendered in front of lower z positions. This is an event subscriber.
    class Renderer {
    private:

        // View matrix is initialized to just being at 0,0
        glm::mat4 m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        Shader* m_Shader;
        VertexBuffer* m_VertexBuffer;
        IndexBuffer* m_IndexBuffer;
        VertexArray* m_VertexArray;
        // These are the raw quads without the model matrix application
        mutable std::vector<std::array<Vertex, 4>> m_Quads;
        // These are the quads that have had the model matrix applied
        mutable std::vector<std::array<Vertex, 4>> m_CalculatedQuads;
        mutable std::vector<glm::mat4> m_ModelMatrices;
        mutable std::vector<Texture*> m_BoundTextures;
        mutable std::map<std::string, Font*> m_Fonts;

        std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int>> m_DeletedQuadIndexes;

    public:

        /// @brief Handles incoming events. Should be registered as a callback to recieve events.
        /// @param event The event the should be handled.
        void OnEvent(Event& event);

    private:

        /// @brief Behavior that should occur on application startup. 
        /// @param xDimension - The x dimension of the renderer window in pixels.
        /// @param yDimension - The y dimension of the renderer window in pixels.
        void OnStart(float xDimension, float yDimension);

        /// @brief Logic that occurs on every application update.
        void OnUpdate();

        /// @brief Logic that needs to occur on application shutdown.
        void OnShutdown();

        /// @brief Adds a new quad of the given size at coordinates 0,0 and a color of white by 
        /// default.
        /// @param size The size of the quad in whatever unit was used to specify the x and y 
        /// dimensions of the Renderer on initialization. It is reccomended that this is in pixels.
        /// @return An ID corresponding to this quad that allows it to be accessed and modified in 
        /// the future.
        unsigned int AddQuad(float size);

        /// @brief Adds a new quad of the given size at coordinates 0,0 with the given texture
        /// @param width The width of the quad in whatever unit was used to specify the x and y 
        /// dimensions of the Renderer on initialization. It is reccomended that this is in pixels.
        /// @param height The height of the quad in whatever unit was used to specify the x and y 
        /// dimensions of the Renderer on initialization. It is reccomended that this is in pixels.
        /// @param textureID the ID of the texture to display
        /// @return An ID corresponding to this quad that allows it to be accessed and modified in 
        /// the future.
        unsigned int AddQuad(float width, float height, unsigned int textureID);

        /// @brief Removes the quad at the given index. The quad will no longer be rendered.
        /// @param quadIndex The quad to be removed.
        /// @return true if the quad is successfully remove, false otherwise
        bool RemoveQuad(const unsigned int quadIndex);

        /// @brief Draws the calculated quads to the screen
        void Draw();

        /// @brief Writes the given string of text in the given font.
        /// @param font An object containing the data for font creation.
        /// @param text The string to render.
        /// @return The ID for the quad the text is rendered on
        unsigned int DrawText(Font& font, const std::string& text);

        /// @brief Sets the model matrix of the quad at a given matrix.
        /// @param quadIndex The index of the quad.
        /// @param modelMatrix The new matrix that should be applied to the quad.
        /// @return true if the model matrix is set successfully, false otherwise
        bool SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix);

        /// @brief Sets the color of the quad to the given color.
        /// @param quadIndex The index of the quad.
        /// @param color The color that the quad should be changed to in RGBA.
        /// @return true if the quad color is set successfully, false otherwise
        bool SetQuadColor(const unsigned int quadIndex, const glm::vec4 color);

        /// @brief Sets the texture coordinates of the quad. 
        /// @param quadIndex The index of the quad.
        /// @param xCoords The x coordinates that correspond to coordinates on this quads texture. 
        /// The vector should order the quads starting with the right and going clockwise.
        /// @param yCoords The y coordinates that correspond to coordinates on this quads texture.
        /// The vector should order the quads starting with the right and going clockwise.
        /// @return true if the texture coords are set successfully, false otherwise
        bool SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords);

        /// @brief Sets the textureID of the quad.
        /// @param quadIndex The index of the quad.
        /// @param textureID The ID of the texture that should be applied to the quad.
        /// @return true if the textureID is set successfully, false otherwise
        bool SetQuadTextureID(const unsigned int quadIndex, const float textureID);

        /// @brief Sets the view matrix of the renderer to the given matrix.
        /// @param viewMatrix The view matrix that should be applied.
        void SetViewMatrix(glm::mat4 viewMatrix);

        /// @brief Updates the texture uniform for the given number of unique textures.
        /// @param uniqueTextures the number of unique textures that are being used
        void UpdateTextureUniform(unsigned int uniqueTextures);

        /// @brief Adds a new texture based on the given path.
        /// @param path the file path to the texture
        /// @return the texture slot that the texture has been bound to
        unsigned int AddTexture(const std::string& path);

        /// @brief Updates the quad at the given index to reflect any any changes to the 
        /// uncalculated quads or modelMatrices. Passes this new data to the GPU
        /// @param index the quad that needs to be updated
        void UpdateCalculatedQuads(const unsigned int index);

        /// @brief Verifies that the index and vertex buffers have enough room to add a quad,
        /// otherwise makes room.
        void ValidateBuffers();

        /// @brief Resize the Renderer viewport.
        /// @param xDimension The width to resize to in pixels.
        /// @param yDimension The height to resize to in pixels.
        void Resize(int xDimension, int yDimension);

    };

}

#endif