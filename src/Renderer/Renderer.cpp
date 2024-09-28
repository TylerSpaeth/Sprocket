#include "Renderer.h"
#include <iostream>
#include <algorithm>
#include "GLValidate.hpp"

// TODO switch to heap allocation to stop stack overflows with large vert counts

static IndexBuffer* GenerateIndexBuffer(unsigned int count) {
  unsigned int indicies[count];
  unsigned int offset = 0;
  for(int i = 0; i < count; i+=6) {
    indicies[i+0] = 0 + offset;
    indicies[i+1] = 1 + offset;
    indicies[i+2] = 2 + offset;
    indicies[i+3] = 2 + offset;
    indicies[i+4] = 3 + offset;
    indicies[i+5] = 0 + offset;
    offset += 4;
  }

  return new IndexBuffer(indicies, count);
}

static std::array<Vertex, 4> CreateQuad(float size, float textureID) {

  // Top Right
  Vertex v0;
  v0.Position = {size/2, size/2, 0.0f};
  v0.Color = {1.0f, 1.0f, 1.0f, 1.0f};
  v0.TextureCoords = {1.0f, 1.0f};
  v0.TextureID = textureID;

  // Bottom Right
  Vertex v1;
  v1.Position = {size/2, -size/2, 0.0f};
  v1.Color = {1.0f, 1.0f, 1.0f, 1.0f};
  v1.TextureCoords = {1.0f, 0.0f};
  v1.TextureID = textureID;

  // Bottom Left
  Vertex v2;
  v2.Position = {-size/2, -size/2, 0.0f};
  v2.Color = {1.0f, 1.0f, 1.0f, 1.0f};
  v2.TextureCoords = {0.0f, 0.0f};
  v2.TextureID = textureID;

  // Top Left
  Vertex v3;
  v3.Position = {-size/2, size/2, 0.0f};
  v3.Color = {1.0f, 1.0f, 1.0f, 1.0f};
  v3.TextureCoords = {0.0f, 1.0f};
  v3.TextureID = textureID;

  return {v0, v1, v2, v3};
}

Renderer::Renderer(const std::string& vertexPath, const std::string& fragmentPath, const unsigned int maxQuads) : m_MaxQuads(maxQuads) {

  // Setup blending
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  m_Shader = new Shader(vertexPath, fragmentPath);
  m_VertexBuffer = new VertexBuffer(nullptr, sizeof(Vertex)*m_MaxQuads * 4);;
  m_IndexBuffer = GenerateIndexBuffer(m_MaxQuads * 6);
  m_VertexArray = new VertexArray();

  // Describes the layout of the VertexBuffer
  VertexBufferLayout layout;
  layout.Push<float>(3); // Coords
  layout.Push<float>(4); // Color
  layout.Push<float>(2); // Texcoords
  layout.Push<float>(1); // TextureID
  m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
}

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////// QUAD ADD AND UPDATE /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

unsigned int Renderer::AddQuad(float size, float textureID) {
  if(m_Quads.size() >= m_MaxQuads) {
    return -1;
  }
  // Create a quad and add it to the back of the quads vector
  auto quad = CreateQuad(size, textureID);
  m_Quads.push_back(quad);
  // Add a new model matrix to the back of the quads vector set to the identity matrix
  m_ModelMatrices.push_back(glm::mat4(1.0f)); 
  // Return the index where the quad and model matrix are set
  return m_Quads.size()-1;
}

// Set the model matrix for the corresponding index, should have scale, rotation,
// and transform already applied
void Renderer::SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix) {
  if(quadIndex == -1) {
    return;
  }
  m_ModelMatrices[quadIndex] = modelMatrix;
}

// Sets the color of the quad at the given index to the given color
void Renderer::SetQuadColor(const unsigned int quadIndex, const glm::vec4 color) {
  auto quad = m_Quads.at(quadIndex);
  for(int i = 0; i < 4; i++) {
    quad.at(i).Color = color;
  }
  m_Quads.at(quadIndex) = quad;
}

// The coords should start in the top right corner and go clockwise
void Renderer::SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords) {
  for(int i = 0; i < 4; i++) {
    m_Quads.at(quadIndex).at(i).TextureCoords = glm::vec2(xCoords[i], yCoords[i]);
  }
}

void Renderer::SetQuadTextureID(const unsigned int quadIndex, const float textureID) {
  for(int i = 0; i < 4; i++) {
    m_Quads.at(quadIndex).at(i).TextureID = textureID;
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Draws all of the quads that have been added to the renderer
void Renderer::Draw() {
  
  // Using this reduces the size of the vertices array if we are not drawing max quads
  Vertex vertices[m_Quads.size() * 4];

  for(int i = 0; i < m_Quads.size(); i++) {
    
    auto quad = m_Quads.at(i);
    auto modelMatrix = m_ModelMatrices.at(i);
    for(int j = 0; j < 4; j++) {
      quad.at(j).Position = modelMatrix * glm::vec4(quad.at(j).Position, 1.0f);
    }
    memcpy(vertices + i * 4, quad.data(), 4 * sizeof(Vertex));

  }
  m_VertexBuffer->Bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
  m_VertexBuffer->Unbind();

  m_Shader->Bind();
	m_VertexArray->Bind();
  m_IndexBuffer->Bind();

  // Update the view matrix uniform
  m_Shader->SetUniformMatrix4f("u_ViewMatrix", m_ViewMatrix);

  glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
}

// Returns the time since this function was last called in terms of microseconds
int64_t Renderer::GetTimeSinceLastChecked() {
  auto currentMicro = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto elapsed = currentMicro - m_LastTimeChecked;
    m_LastTimeChecked = currentMicro;
    return elapsed;
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// SHADER UNIFORMS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

void Renderer::SetViewMatrix(glm::mat4 viewMatrix) {
  m_ViewMatrix = viewMatrix;
}

void Renderer::SetProjectionMatrix(const glm::mat4& projectionMatrix) {
  m_Shader->Bind();
  m_Shader->SetUniformMatrix4f("u_ProjectionMatrix", projectionMatrix);
  m_Shader->Unbind();
}

// This updates the texture uniform to have IDs for all the textures
// that need to be rendered, asssuming that they start at slot 1 and are
// in sequencial order
void Renderer::UpdateTextureUniform(unsigned int uniqueTextures) {
  m_Shader->Bind();
  int textureIDs[uniqueTextures];
  for(int i = 1; i <= uniqueTextures; i++) {
    textureIDs[i-1] = i;
  }
  m_Shader->SetUniform1iv("u_Texture", uniqueTextures, textureIDs);
  m_Shader->Unbind();
}
