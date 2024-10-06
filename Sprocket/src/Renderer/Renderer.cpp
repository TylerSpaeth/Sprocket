#include "Renderer.h"

#include "ThirdParty/glad/glad.h"

namespace Sprocket {

  ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// NONCLASS FUNCTIONS ////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  static IndexBuffer* GenerateIndexBuffer(unsigned int count) {
    unsigned int* indicies = (unsigned int*) malloc(count*sizeof(unsigned int));
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

    IndexBuffer* ib = new IndexBuffer(indicies, count);
    free(indicies);

    return ib;
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
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////// STATIC FUNCTIONS /////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  Renderer* Renderer::s_Instance = nullptr;
  void Renderer::Init(const unsigned int maxQuads, const unsigned int xDimension, const unsigned int yDimension) {
    if(!s_Instance) {
      s_Instance = new Renderer(maxQuads);

      // Setup blending
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);

      s_Instance->m_VertexBuffer = new VertexBuffer(nullptr, sizeof(Vertex)*s_Instance->m_MaxQuads * 4);;
      s_Instance->m_IndexBuffer = GenerateIndexBuffer(s_Instance->m_MaxQuads * 6);
      s_Instance->m_VertexArray = new VertexArray();
      s_Instance->m_Shader = new Shader("Default.vert", "Default.frag");

      s_Instance->m_VertexArray->Bind();
      s_Instance->m_VertexBuffer->Bind();
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10*sizeof(float), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10*sizeof(float), (void*)(3*sizeof(float)));
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10*sizeof(float), (void*)(7*sizeof(float)));
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10*sizeof(float), (void*)(9*sizeof(float)));
      s_Instance->m_VertexBuffer->Unbind();
      s_Instance->m_VertexArray->Unbind();

       // By default, the projection matrix will have the center of the screen at (0,0) and
      // the the edges will be determined by the x and y values given 
      s_Instance->m_Shader->Bind();
      s_Instance->m_Shader->SetUniformMatrix4f("u_ProjectionMatrix", glm::ortho(-(float)xDimension/2, (float)xDimension/2, -(float) yDimension/2, (float)yDimension/2));
      s_Instance->m_Shader->Unbind();
    }
  }

  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// INSTANCE FUNCTIONS ////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  // Draws all of the quads that have been added to the renderer
  void Renderer::Draw() {
    
    // Reserve a Vertex vector with enough room for all the quads
    std::vector<Vertex> vertices;
    vertices.reserve(m_Quads.size() * 4);

    // Loop through each quad and apply its model matrix, then push it to the vecor
    for(int i = 0; i < m_Quads.size(); i++) {
      auto quad = m_Quads.at(i);
      auto modelMatrix = m_ModelMatrices.at(i);
      for(int j = 0; j < 4; j++) {
        quad.at(j).Position = modelMatrix * glm::vec4(quad.at(j).Position, 1.0f);
        vertices.push_back(quad.at(j));
      }
    }
    // Send the data from the vector to the GPU for rendering
    m_VertexBuffer->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), &vertices.front());
    m_VertexBuffer->Unbind();
    
    

    m_Shader->Bind();
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();

    // Update the view matrix uniform
    m_Shader->SetUniformMatrix4f("u_ViewMatrix", m_ViewMatrix);
    // To send the model matrix data to the gpu for processesing
    //m_Shader->SetUniformMatrix4fv("u_ModelMatrix", m_ModelMatrices.size(), m_ModelMatrices.front());

    glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);
  }

  ///////////////////////////////// EVENT FUNCTIONS /////////////////////////////////

  void Renderer::OnEventInstance(Event& event) {
    EventType type = event.GetEventType();
    switch(type) {
      case APP_UPDATE:
        OnUpdateInstance();
        break;
      case WINDOW_CLOSE:
        OnCloseInstance();
        break;
    }
  }

  void Renderer::OnUpdateInstance() {
    glClear(GL_COLOR_BUFFER_BIT);
    Draw();
  }

  void Renderer::OnCloseInstance() {
    // TODO make sure everything is getting cleaned up
    delete m_VertexBuffer;
    delete m_VertexArray;
    delete m_IndexBuffer;
    delete m_Shader;
    // Deallocate all of the textures pointers
    for(Texture* t : m_BoundTextures) {
      free(t);
    }
    // Clear Vectors
    m_Quads.clear();
    m_ModelMatrices.clear();
    m_BoundTextures.clear();
  }

  ////////////////////////////////// QUAD FUNCTIONS //////////////////////////////////

  unsigned int Renderer::AddQuadInstance(float size, float textureID) {
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
  void Renderer::SetQuadModelMatrixInstance(const unsigned int quadIndex, const glm::mat4 modelMatrix) {
    if(quadIndex == -1) {
      return;
    }
    m_ModelMatrices[quadIndex] = modelMatrix;
  }

  // Sets the color of the quad at the given index to the given color
  void Renderer::SetQuadColorInstance(const unsigned int quadIndex, const glm::vec4 color) {
    auto quad = m_Quads.at(quadIndex);
    for(int i = 0; i < 4; i++) {
      quad.at(i).Color = color;
    }
    m_Quads.at(quadIndex) = quad;
  }

  // The coords should start in the top right corner and go clockwise
  void Renderer::SetQuadTextureCoordsInstance(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords) {
    for(int i = 0; i < 4; i++) {
      m_Quads.at(quadIndex).at(i).TextureCoords = glm::vec2(xCoords[i], yCoords[i]);
    }
  }

  void Renderer::SetQuadTextureIDInstance(const unsigned int quadIndex, const float textureID) {
    for(int i = 0; i < 4; i++) {
      m_Quads.at(quadIndex).at(i).TextureID = textureID;
    }
  }

  ///////////////////////////// SHADER UNIFORM FUNCTIONS /////////////////////////////

  void Renderer::SetViewMatrixInstance(glm::mat4 viewMatrix) {
    m_ViewMatrix = viewMatrix;
  }

  // This updates the texture uniform to have IDs for all the textures
  // that need to be rendered, asssuming that they start at slot 1 and are
  // in sequencial order
  void Renderer::UpdateTextureUniformInstance(unsigned int uniqueTextures) {
    m_Shader->Bind();
    int textureIDs[uniqueTextures];
    for(int i = 1; i <= uniqueTextures; i++) {
      textureIDs[i-1] = i;
    }
    m_Shader->SetUniform1iv("u_Texture", uniqueTextures, textureIDs);
    m_Shader->Unbind();
  }

  void Renderer::AddTextureInstance(const std::string& path, unsigned int slot) {
    Texture* texture = new Texture(path, slot);
    m_BoundTextures.push_back(texture);
    texture->Bind(slot);
  }

  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

}
