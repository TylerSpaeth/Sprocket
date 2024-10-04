#ifndef RENDERER_H
#define RENDERER_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "../ThirdParty/glm/glm.hpp"
#include "../ThirdParty/glm/gtc/matrix_transform.hpp"
#include "../Events/Event.h"

#include "../Macros.h"

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
      mutable std::vector<std::array<Vertex, 4>> m_Quads;
      mutable std::vector<glm::mat4> m_ModelMatrices;
      void Draw();
      ~Renderer();

      // Singleton Components
      static Renderer* s_Instance;
      Renderer(unsigned int maxQuads) : m_MaxQuads(maxQuads){}
      Renderer(const Renderer&) = delete;
      Renderer operator=(const Renderer&) = delete;

      // Actual implementations for the static instance functions
      void OnEventInstance(Event& event);
      void OnUpdateInstance();
      unsigned int AddQuadInstance(float size, float textureID);
      void SetQuadModelMatrixInstance(const unsigned int quadIndex, const glm::mat4 modelMatrix);
      void SetQuadColorInstance(const unsigned int quadIndex, const glm::vec4 color);
      void SetQuadTextureCoordsInstance(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords);
      void SetQuadTextureIDInstance(const unsigned int quadIndex, const float textureID);
      void SetViewMatrixInstance(glm::mat4 viewMatrix);
      void UpdateTextureUniformInstance(unsigned int uniqueTextures);

    public:
      static void Init(const unsigned int maxQuads, const unsigned int xDimension, const unsigned int yDimension);
      static void OnEvent(Event& event) {s_Instance->OnEventInstance(event);}
      static void OnUpdate() {s_Instance->OnUpdateInstance();}
      static unsigned int AddQuad(float size, float textureID) {return s_Instance->AddQuadInstance(size, textureID);}
      static void SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix) {s_Instance->SetQuadModelMatrixInstance(quadIndex, modelMatrix);}
      static void SetQuadColor(const unsigned int quadIndex, const glm::vec4 color) {s_Instance->SetQuadColorInstance(quadIndex, color);}
      static void SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords) {s_Instance->SetQuadTextureCoordsInstance(quadIndex, xCoords, yCoords);}
      static void SetQuadTextureID(const unsigned int quadIndex, const float textureID) {s_Instance->SetQuadTextureIDInstance(quadIndex, textureID);}
      static void SetViewMatrix(glm::mat4 viewMatrix) {s_Instance->SetViewMatrixInstance(viewMatrix);}
      static void UpdateTextureUniform(unsigned int uniqueTextures) {s_Instance->UpdateTextureUniformInstance(uniqueTextures);}
  };

}

#endif