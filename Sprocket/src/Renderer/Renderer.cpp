#include "Renderer.h"

#include "Core/Global.h"

#include "Events/RenderEvent.h"

#include "ThirdParty/glad/glad.h"

#include "Core/Sprocket.pch"

namespace Sprocket {

    const float Renderer::s_PixelsPerUnit = 100;

    Vertex ClearedVertex = { {0,0,0},{0,0,0,0},{0,0},0 };
    std::array<Vertex, 4> ClearedQuad = { ClearedVertex, ClearedVertex, ClearedVertex, ClearedVertex };

    ////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////// NONCLASS FUNCTIONS ////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    static IndexBuffer* GenerateIndexBuffer(unsigned int count) {
        if(count < 6 || count % 6 != 0) {
            Global::fileLogger.Error("The number of verticies in an index buffer must be a positive multiple of 6.");
            return nullptr;
        }
        unsigned int* indicies = new unsigned int[count];
        //unsigned int* indicies = (unsigned int*)malloc(count * sizeof(unsigned int));
        unsigned int offset = 0;
        for (int i = 0; i < count; i += 6) {
            indicies[i + 0] = 0 + offset;
            indicies[i + 1] = 1 + offset;
            indicies[i + 2] = 2 + offset;
            indicies[i + 3] = 2 + offset;
            indicies[i + 4] = 3 + offset;
            indicies[i + 5] = 0 + offset;
            offset += 4;
        }

        IndexBuffer* ib = new IndexBuffer(indicies, count);
        delete[] indicies;

        return ib;
    }

    static std::array<Vertex, 4> CreateQuad(float size, float textureID) {

        // Top Right
        Vertex v0;
        v0.Position = { size / 2, size / 2, 0.0f };
        v0.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v0.TextureCoords = { 1.0f, 1.0f };
        v0.TextureID = textureID;

        // Bottom Right
        Vertex v1;
        v1.Position = { size / 2, -size / 2, 0.0f };
        v1.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v1.TextureCoords = { 1.0f, 0.0f };
        v1.TextureID = textureID;

        // Bottom Left
        Vertex v2;
        v2.Position = { -size / 2, -size / 2, 0.0f };
        v2.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v2.TextureCoords = { 0.0f, 0.0f };
        v2.TextureID = textureID;

        // Top Left
        Vertex v3;
        v3.Position = { -size / 2, size / 2, 0.0f };
        v3.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v3.TextureCoords = { 0.0f, 1.0f };
        v3.TextureID = textureID;

        return { v0, v1, v2, v3 };
    }

    static bool IsQuadEmpty(std::array<Vertex, 4> quad) {
        return quad[0] == ClearedVertex &&
            quad[1] == ClearedVertex &&
            quad[2] == ClearedVertex &&
            quad[3] == ClearedVertex;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    ////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////// STATIC FUNCTIONS /////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    Renderer* Renderer::s_Instance = nullptr;
    void Renderer::Init(const unsigned int maxQuads, const unsigned int xDimension, const unsigned int yDimension) {
        if (!s_Instance) {

            if (xDimension < 0 || yDimension < 0) {
                throw std::invalid_argument("Renderer dimensions can not be negative.");
            }

            s_Instance = new Renderer(maxQuads);

            // Setup blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);

            // Allow sorting by z position
            glEnable(GL_DEPTH_TEST);

            s_Instance->m_VertexBuffer = new VertexBuffer(nullptr, sizeof(Vertex) * s_Instance->m_MaxQuads * 4);;
            s_Instance->m_IndexBuffer = GenerateIndexBuffer(s_Instance->m_MaxQuads * 6);
            s_Instance->m_VertexArray = new VertexArray();

            // Check to see how many texture slots the system has
            int systemMaxTextures;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &systemMaxTextures);
            // Use the appropriate fragment shader for the number of texture slots the system has
            // TODO add more options
            if (systemMaxTextures < 32) {
                s_Instance->m_Shader = new Shader("shaders/Default.vert", "shaders/Default.frag");
            }
            else {
                s_Instance->m_Shader = new Shader("shaders/Default.vert", "shaders/Default32.frag");
            }

            // Define the layout of the vertex buffer
            s_Instance->m_VertexArray->Bind();
            s_Instance->m_VertexBuffer->Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
            s_Instance->m_VertexBuffer->Unbind();
            s_Instance->m_VertexArray->Unbind();

            // By default, the projection matrix will have the center of the screen at (0,0) and
           // the the edges will be determined by the x and y values given 
            s_Instance->m_Shader->Bind();
            s_Instance->m_Shader->SetUniformMatrix4f("u_ProjectionMatrix", glm::ortho(-(float)xDimension / 2, (float)xDimension / 2, -(float)yDimension / 2, (float)yDimension / 2));
            s_Instance->m_Shader->Unbind();

            s_Instance->m_CalculatedQuads.reserve(s_Instance->m_MaxQuads * 4);
        }
    }

    ///////////////////////////////// EVENT FUNCTIONS /////////////////////////////////

    void Renderer::OnEvent(Event& event) {
        EventType type = event.GetEventType();
        switch (type) {
        case EventType::APP_UPDATE:
            OnUpdate();
            break;
        case EventType::WINDOW_CLOSE:
            OnClose();
            break;
        case EventType::RENDER_NEW:
            ((RenderNewEvent&)event).m_QuadID = AddQuad(s_PixelsPerUnit);
            break;
        case EventType::RENDER_UPDATE: {
            switch (((RenderUpdateEvent&)event).GetType()) {
            case RenderUpdateType::VIEW_MATRIX:
                SetViewMatrix(((RenderUpdateEvent&)event).m_Matrix);
                break;
            case RenderUpdateType::MODEL_MATRIX:
                SetQuadModelMatrix(((RenderUpdateEvent&)event).m_QuadID, ((RenderUpdateEvent&)event).m_Matrix);
                break;
            case RenderUpdateType::QUAD:
                unsigned int slot = 0;
                if (((RenderUpdateEvent&)event).m_TexturePath != "") {
                    slot = AddTexture(((RenderUpdateEvent&)event).m_TexturePath);
                }
                SetQuadTextureID(((RenderUpdateEvent&)event).m_QuadID, slot);
                UpdateTextureUniform(s_Instance->m_BoundTextures.size());
                SetQuadColor(((RenderUpdateEvent&)event).m_QuadID, ((RenderUpdateEvent&)event).m_QuadColor);
                SetQuadTextureCoords(((RenderUpdateEvent&)event).m_QuadID, ((RenderUpdateEvent&)event).m_TexXCoords, ((RenderUpdateEvent&)event).m_TexYCoords);
                break;
            }
            break;
        }
        case EventType::RENDER_DELETE:
            RemoveQuad(((RenderDeleteEvent&)event).m_QuadID);
            break;

        }
    }

    void Renderer::OnUpdate() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        s_Instance->Draw();
    }

    void Renderer::OnClose() {
        // TODO make sure everything is getting cleaned up
        delete s_Instance->m_VertexBuffer;
        delete s_Instance->m_VertexArray;
        delete s_Instance->m_IndexBuffer;
        delete s_Instance->m_Shader;
        // Deallocate all of the textures pointers
        for (Texture* t : s_Instance->m_BoundTextures) {
            delete t;
        }
        // Clear Vectors
        s_Instance->m_Quads.clear();
        s_Instance->m_CalculatedQuads.clear();
        s_Instance->m_ModelMatrices.clear();
        s_Instance->m_BoundTextures.clear();
    }

    ////////////////////////////////// QUAD FUNCTIONS //////////////////////////////////

    unsigned int Renderer::AddQuad(float size) {

        if (size <= 0) {
            throw std::invalid_argument("A quad must have a positive, nonzero size.");
        }

        if (s_Instance->m_Quads.size() - s_Instance->m_DeletedQuadIndexes.size() >= s_Instance->m_MaxQuads) {
            throw std::runtime_error("The maximum number of quads has already been reached. To use more quads, change the increase the amount at intialization.");
        }

        auto quad = CreateQuad(size, 0);

        // If there are no gaps in the vector then just push to the back
        if (s_Instance->m_DeletedQuadIndexes.size() == 0) {
            s_Instance->m_Quads.push_back(quad);
            s_Instance->m_CalculatedQuads.push_back(quad);
            // Add a new model matrix to the back of the quads vector set to the identity matrix
            s_Instance->m_ModelMatrices.push_back(glm::mat4(1.0f));

            s_Instance->UpdateCalculatedQuads(s_Instance->m_Quads.size() - 1);
            // Return the index where the quad and model matrix are set
            return s_Instance->m_Quads.size() - 1;
        }

        // Find the next deleted Quad in the vector
        auto nextOpen = s_Instance->m_DeletedQuadIndexes.top();
        s_Instance->m_Quads.at(nextOpen) = quad;
        s_Instance->UpdateCalculatedQuads(nextOpen);
        s_Instance->m_DeletedQuadIndexes.pop();
        return nextOpen;

        return -1; // Some kind of error occured if this is reached
    }

    // FIXME deleted quads can still be modified by using the index in another function. It is not 
    // crucial to fix this immediately, but this will cause problems if the index of a deleted quad 
    // is used because the number of deleted quads will not equal the running count. This will 
    // eventually result in errors adding quads since a new quad may not be added and -1 will be 
    // returned. This can probably be solved quickly by checking an index against the indexes of 
    // deleted quads before performing any operations.

    bool Renderer::RemoveQuad(const unsigned int quadIndex) {
        try {
            s_Instance->m_Quads.at(quadIndex) = ClearedQuad;
            s_Instance->m_CalculatedQuads.at(quadIndex) = ClearedQuad;
            s_Instance->m_ModelMatrices.at(quadIndex) = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
            s_Instance->m_DeletedQuadIndexes.push(quadIndex);
            UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;

    }

    // Set the model matrix for the corresponding index, should have scale, rotation,
    // and transform already applied
    bool Renderer::SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix) {
        try {

            glm::mat4 scaledModelMatrix = modelMatrix;

            glm::vec3 position = modelMatrix[3];
            position *= s_PixelsPerUnit;
            scaledModelMatrix[3] = glm::vec4(position, 1.0f);

            s_Instance->m_ModelMatrices[quadIndex] = scaledModelMatrix;
            s_Instance->UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    // Sets the color of the quad at the given index to the given color
    bool Renderer::SetQuadColor(const unsigned int quadIndex, const glm::vec4 color) {
        try {
            auto quad = s_Instance->m_Quads.at(quadIndex);
            for (int i = 0; i < 4; i++) {
                quad.at(i).Color = color;
            }
            s_Instance->m_Quads.at(quadIndex) = quad;
            s_Instance->UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    // The coords should start in the top right corner and go clockwise
    bool Renderer::SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords) {
        try {
            for (int i = 0; i < 4; i++) {
                s_Instance->m_Quads.at(quadIndex).at(i).TextureCoords = glm::vec2(xCoords[i], yCoords[i]);
            }
            s_Instance->UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    bool Renderer::SetQuadTextureID(const unsigned int quadIndex, const float textureID) {
        try {
            for (int i = 0; i < 4; i++) {
                s_Instance->m_Quads.at(quadIndex).at(i).TextureID = textureID;
            }
            s_Instance->UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    ///////////////////////////// SHADER UNIFORM FUNCTIONS /////////////////////////////

    void Renderer::SetViewMatrix(glm::mat4 viewMatrix) {

        glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
        glm::vec3 position = glm::vec3(inverseViewMatrix[3]);
        position *= s_PixelsPerUnit;
        glm::vec3 forward = glm::vec3(-inverseViewMatrix[2]);
        glm::vec3 target = position + forward;
        glm::vec3 up = glm::vec3(inverseViewMatrix[1]);

        s_Instance->m_ViewMatrix = glm::lookAt(position, target, up);
    }

    // This updates the texture uniform to have IDs for all the textures
    // that need to be rendered, asssuming that they start at slot 1 and are
    // in sequencial order
    void Renderer::UpdateTextureUniform(unsigned int uniqueTextures) {
        s_Instance->m_Shader->Bind();
        std::vector<int> textureIDs(uniqueTextures);
        for (int i = 1; i <= uniqueTextures; i++) {
            textureIDs[i - 1] = i;
        }
        s_Instance->m_Shader->SetUniform1iv("u_Texture", uniqueTextures, textureIDs.data());
        s_Instance->m_Shader->Unbind();
    }

    unsigned int Renderer::AddTexture(const std::string& path) {

        for (Texture* t : s_Instance->m_BoundTextures) {
            if (path.compare(t->m_FilePath) == 0) {
                return t->m_Slot;
            }
        }

        Texture* texture = new Texture(path);
        s_Instance->m_BoundTextures.push_back(texture);
        texture->Bind(s_Instance->m_BoundTextures.size());
        return s_Instance->m_BoundTextures.size();
    }

    void Renderer::UpdateCalculatedQuads(const unsigned int index) {
        auto quad = s_Instance->m_Quads.at(index);
        auto modelMatrix = s_Instance->m_ModelMatrices.at(index);
        for (int j = 0; j < 4; j++) {
            quad[j].Position = modelMatrix * glm::vec4(quad.at(j).Position, 1.0f);
        }

        s_Instance->m_CalculatedQuads[index] = quad;

        // Update the GPU's data to reflect this
        s_Instance->m_VertexBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, index * 4 * sizeof(Vertex), sizeof(Vertex) * 4, &s_Instance->m_CalculatedQuads[index]);
        s_Instance->m_VertexBuffer->Unbind();
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

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

}
