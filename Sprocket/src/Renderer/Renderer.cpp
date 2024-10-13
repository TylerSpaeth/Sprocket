#include "Renderer.h"

#include "Events/RenderEvent.h"

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

      // Check to see how many texture slots the system has
      int systemMaxTextures;
      glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &systemMaxTextures);
      // Use the appropriate fragment shader for the number of texture slots the system has
      // TODO add more options
      if(systemMaxTextures < 32) {
        s_Instance->m_Shader = new Shader("Default.vert", "Default.frag");
      }
      else {
        s_Instance->m_Shader = new Shader("Default.vert", "Default32.frag");
      }

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

      s_Instance->m_CalculatedQuads.reserve(s_Instance->m_MaxQuads * 4);
    }
  }

  ///////////////////////////////// EVENT FUNCTIONS /////////////////////////////////

  void Renderer::OnEvent(Event& event) {
    EventType type = event.GetEventType();
    switch(type) {
      case EventType::APP_UPDATE:
        OnUpdate();
        break;
      case EventType::WINDOW_CLOSE:
        OnClose();
        break;
      case EventType::RENDER_NEW:
        ((RenderNewEvent&) event).m_QuadID = AddQuad(((RenderNewEvent&) event).GetSize(), ((RenderNewEvent&) event).GetTextureID());
        break;
      case EventType::RENDER_UPDATE: {
        switch(((RenderUpdateEvent&)event).GetType()) {
          case RenderUpdateType::VIEW_MATRIX:
            SetViewMatrix(((RenderUpdateEvent&)event).m_Matrix);
            break;
          case RenderUpdateType::MODEL_MATRIX:
            SetQuadModelMatrix(((RenderUpdateEvent&)event).m_QuadIndex, ((RenderUpdateEvent&)event).m_Matrix);
            break;
          case RenderUpdateType::QUAD_TEX_ID:
            SetQuadTextureID(((RenderUpdateEvent&)event).m_QuadIndex, ((RenderUpdateEvent&)event).m_TextureID);
            break;
          case RenderUpdateType::QUAD_COLOR:
            SetQuadColor(((RenderUpdateEvent&)event).m_QuadIndex, ((RenderUpdateEvent&)event).m_Vec1);
            break;
          case RenderUpdateType::QUAD_TEX_COORDS:
            SetQuadTextureCoords(((RenderUpdateEvent&)event).m_QuadIndex, ((RenderUpdateEvent&)event).m_Vec1, ((RenderUpdateEvent&)event).m_Vec2);
          break;
        }
      }

    }
  }

  void Renderer::OnUpdate() {
    glClear(GL_COLOR_BUFFER_BIT);
    s_Instance->Draw();
  }

  void Renderer::OnClose() {
    // TODO make sure everything is getting cleaned up
    delete s_Instance->m_VertexBuffer;
    delete s_Instance->m_VertexArray;
    delete s_Instance->m_IndexBuffer;
    delete s_Instance->m_Shader;
    // Deallocate all of the textures pointers
    for(Texture* t : s_Instance->m_BoundTextures) {
      free(t);
    }
    // Clear Vectors
    s_Instance->m_Quads.clear();
    s_Instance->m_ModelMatrices.clear();
    s_Instance->m_BoundTextures.clear();
  }

  ////////////////////////////////// QUAD FUNCTIONS //////////////////////////////////

  unsigned int Renderer::AddQuad(float size, float textureID) {
    if(s_Instance->m_Quads.size() >= s_Instance->m_MaxQuads) {
      return -1;
    }
    // Create a quad and add it to the back of the quads vector
    auto quad = CreateQuad(size, textureID);
    s_Instance->m_Quads.push_back(quad);
    s_Instance->m_CalculatedQuads.push_back(quad);
    // Add a new model matrix to the back of the quads vector set to the identity matrix
    s_Instance->m_ModelMatrices.push_back(glm::mat4(1.0f)); 

    s_Instance->UpdateCalculatedQuads(s_Instance->m_Quads.size()-1);
    // Return the index where the quad and model matrix are set
    return s_Instance->m_Quads.size()-1;
  }

  // Set the model matrix for the corresponding index, should have scale, rotation,
  // and transform already applied
  void Renderer::SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix) {
    if(quadIndex == -1) {
      return;
    }
    s_Instance->m_ModelMatrices[quadIndex] = modelMatrix;
    s_Instance->UpdateCalculatedQuads(quadIndex);
  }

  // Sets the color of the quad at the given index to the given color
  void Renderer::SetQuadColor(const unsigned int quadIndex, const glm::vec4 color) {
    auto quad = s_Instance->m_Quads.at(quadIndex);
    for(int i = 0; i < 4; i++) {
      quad.at(i).Color = color;
    }
    s_Instance->m_Quads.at(quadIndex) = quad;
    s_Instance->UpdateCalculatedQuads(quadIndex);
  }

  // The coords should start in the top right corner and go clockwise
  void Renderer::SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords) {
    for(int i = 0; i < 4; i++) {
      s_Instance->m_Quads.at(quadIndex).at(i).TextureCoords = glm::vec2(xCoords[i], yCoords[i]);
    }
    s_Instance->UpdateCalculatedQuads(quadIndex);
  }

  void Renderer::SetQuadTextureID(const unsigned int quadIndex, const float textureID) {
    for(int i = 0; i < 4; i++) {
      s_Instance->m_Quads.at(quadIndex).at(i).TextureID = textureID;
    }
    s_Instance->UpdateCalculatedQuads(quadIndex);
  }

  ///////////////////////////// SHADER UNIFORM FUNCTIONS /////////////////////////////

  void Renderer::SetViewMatrix(glm::mat4 viewMatrix) {
    s_Instance->m_ViewMatrix = viewMatrix;
  }

  // This updates the texture uniform to have IDs for all the textures
  // that need to be rendered, asssuming that they start at slot 1 and are
  // in sequencial order
  void Renderer::UpdateTextureUniform(unsigned int uniqueTextures) {
    s_Instance->m_Shader->Bind();
    int textureIDs[uniqueTextures];
    for(int i = 1; i <= uniqueTextures; i++) {
      textureIDs[i-1] = i;
    }
    s_Instance->m_Shader->SetUniform1iv("u_Texture", uniqueTextures, textureIDs);
    s_Instance->m_Shader->Unbind();
  }

  void Renderer::AddTexture(const std::string& path, unsigned int slot) {
    Texture* texture = new Texture(path, slot);
    s_Instance->m_BoundTextures.push_back(texture);
    texture->Bind(slot);
  }

  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////// INSTANCE FUNCTIONS ////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

  // Draws all of the quads that have been added to the renderer
  void Renderer::Draw() {
    m_Shader->Bind();
    m_VertexArray->Bind();
    m_IndexBuffer->Bind();

    // Update the view matrix uniform
    m_Shader->SetUniformMatrix4f("u_ViewMatrix", m_ViewMatrix);
    // To send the model matrix data to the gpu for processesing
    //m_Shader->SetUniformMatrix4fv("u_ModelMatrix", m_ModelMatrices.size(), m_ModelMatrices.front());

    glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, 0);

    m_IndexBuffer->Unbind();
    m_VertexArray->Unbind();
    m_Shader->Unbind();
  }

  void Renderer::UpdateCalculatedQuads(const unsigned int index) {
    auto quad = m_Quads.at(index);
    auto modelMatrix = m_ModelMatrices.at(index);
    for(int j = 0; j < 4; j++) {
      quad[j].Position = modelMatrix * glm::vec4(quad.at(j).Position, 1.0f);
    }

    m_CalculatedQuads[index] = quad;

    // Update the GPU's data to reflect this
    m_VertexBuffer->Bind();
    glBufferSubData(GL_ARRAY_BUFFER, index*4*sizeof(Vertex), sizeof(Vertex) * 4, &m_CalculatedQuads[index]);
    m_VertexBuffer->Unbind();
  }

  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////////

}
