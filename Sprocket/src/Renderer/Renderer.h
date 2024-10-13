#ifndef RENDERER_H
#define RENDERER_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"
#include "Events/Event.h"

#include "Core/Macros.h"

#include <array>
#include <vector>

namespace Sprocket {

  struct Vertex {
    glm::vec3 Position;
    glm::vec4 Color;
    glm::vec2 TextureCoords;
    float TextureID;
  };

  // TODO the texture and shader system for the renderer needs rework

  /// @brief This is a singleton that handles all of the rendering functionality for a Sprocket
  /// application using OpenGL. This is an event subscriber.
  class SPROCKET_API Renderer {
    private:
      // View matrix is initialized to just being at 0,0
      glm::mat4 m_ViewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
      Shader* m_Shader;
      unsigned int m_MaxQuads;
      VertexBuffer* m_VertexBuffer;
      IndexBuffer* m_IndexBuffer;
      VertexArray* m_VertexArray;
      // These are the raw quads without the model matrix application
      mutable std::vector<std::array<Vertex, 4>> m_Quads; 
      // These are the quads that have had the model matrix applied
      mutable std::vector<std::array<Vertex, 4>> m_CalculatedQuads;
      mutable std::vector<glm::mat4> m_ModelMatrices;
      mutable std::vector<Texture*> m_BoundTextures;
      void Draw();

      /// @brief Updates the quad at the given index to reflect any any changes to the 
      /// uncalculated quads or modelMatrices. Passes this new data to the GPU
      /// @param index the quad that needs to be updated
      void UpdateCalculatedQuads(const unsigned int index);

      // Singleton Components
      static Renderer* s_Instance;
      Renderer(unsigned int maxQuads) : m_MaxQuads(maxQuads){}
      Renderer(const Renderer&) = delete;
      Renderer operator=(const Renderer&) = delete;

      static void OnUpdate();
      static void OnClose();

      /// @brief Adds a new quads of the given size at coordinates 0,0 with a texture corresponding
      /// textureID and a color of white be default.
      /// @param size The size of the quad in whatever unit was used to specify the x and y 
      /// dimensions of the Renderer on initialization. It is reccomended that this is in pixels.
      /// @param textureID The id that corresponds to the texture slot where a given texture is 
      /// stored.
      /// @return An ID corresponding to this quad that allows it to be accessed and modified in 
      /// the future.
      static unsigned int AddQuad(float size, float textureID);

      /// @brief Sets the model matrix of the quad at a given matrix.
      /// @param quadIndex The index of the quad.
      /// @param modelMatrix The new matrix that should be applied to the quad.
      static void SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix);

      /// @brief Sets the color of the quad to the given color.
      /// @param quadIndex The index of the quad.
      /// @param color The color that the quad should be changed to in RGBA.
      static void SetQuadColor(const unsigned int quadIndex, const glm::vec4 color);

      /// @brief Sets the texture coordinates of the quad. 
      /// @param quadIndex The index of the quad.
      /// @param xCoords The x coordinates that correspond to coordinates on this quads texture. 
      /// The vector should order the quads starting with the right and going clockwise.
      /// @param yCoords The y coordinates that correspond to coordinates on this quads texture.
      /// The vector should order the quads starting with the right and going clockwise.
      static void SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords);

      /// @brief Sets the textureID of the quad.
      /// @param quadIndex The index of the quad.
      /// @param textureID The ID of the texture that should be applied to the quad.
      static void SetQuadTextureID(const unsigned int quadIndex, const float textureID);

      /// @brief Sets the view matrix of the renderer to the given matrix.
      /// @param viewMatrix The view matrix that should be applied.
      static void SetViewMatrix(glm::mat4 viewMatrix);

    public:
      /// @brief Initializes the singleton and sets the appropriate parameters. This must be called
      /// before any other Renderer function.
      /// @param maxQuads the maximum number of quads that the Renderer should accept.
      /// @param xDimension The width of the projection matrix. It is recommended that this value 
      /// the same as the width of the window's native resolution in pixels.
      /// @param yDimension The height of the projection matrix. It is recommended that this value 
      /// the same as the height of the window's native resolution in pixels.
      static void Init(const unsigned int maxQuads, const unsigned int xDimension, const unsigned int yDimension);

      /// @brief Handles incoming events. Should be registered as a callback to recieve events.
      /// @param event The event the should be handled.
      static void OnEvent(Event& event);

      static void UpdateTextureUniform(unsigned int uniqueTextures);
      static void AddTexture(const std::string& path, unsigned int slot);
  };

}

#endif