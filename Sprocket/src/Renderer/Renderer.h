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
      void UpdateCalculatedQuads(const unsigned int index);

      // Singleton Components
      static Renderer* s_Instance;
      Renderer(unsigned int maxQuads) : m_MaxQuads(maxQuads){}
      Renderer(const Renderer&) = delete;
      Renderer operator=(const Renderer&) = delete;

      static void OnUpdate();
      static void OnClose();

    public:
      static void Init(const unsigned int maxQuads, const unsigned int xDimension, const unsigned int yDimension);
      static void OnEvent(Event& event);
      static unsigned int AddQuad(float size, float textureID);
      static void SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix);
      static void SetQuadColor(const unsigned int quadIndex, const glm::vec4 color);
      static void SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords);
      static void SetQuadTextureID(const unsigned int quadIndex, const float textureID);
      static void SetViewMatrix(glm::mat4 viewMatrix);
      static void UpdateTextureUniform(unsigned int uniqueTextures);
      static void AddTexture(const std::string& path, unsigned int slot);
  };

}

#endif