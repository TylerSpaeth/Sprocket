#ifndef RENDERER_H
#define RENDERER_H

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/matrix_transform.hpp"

#include <array>
#include <vector>

struct Vertex {
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TextureCoords;
  float TextureID;
};

class Renderer {
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
  public:
    Renderer(const unsigned int maxQuads);
    void AttachShader(Shader* shader);
    void Clear() const;
    unsigned int AddQuad(float size, float textureID);
    void SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix);
    void SetQuadColor(const unsigned int quadIndex, const glm::vec4 color);
    void SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords);
    void SetQuadTextureID(const unsigned int quadIndex, const float textureID);
    void Draw();
    void SetViewMatrix(glm::mat4 viewMatrix);
    void SetProjectionMatrix(const glm::mat4& projectionMatrix);;
    void UpdateTextureUniform(unsigned int uniqueTextures);
};

#endif