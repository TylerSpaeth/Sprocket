#include "Renderer.h"
#include "VertexBuffer.h"

#include "Core/Global.h"
#include "Core/Sprocket.pch"

#include "Events/ApplicationEvent.h"
#include "Events/RenderEvent.h"
#include "Events/WindowEvent.h"

#include "ThirdParty/glad/glad.h"

namespace Sprocket {

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////NONCLASS DEFINITIONS/////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    Vertex ClearedVertex = { {0,0,0},{0,0,0,0},{0,0},0 };
    std::array<Vertex, 4> ClearedQuad = { ClearedVertex, ClearedVertex, ClearedVertex, ClearedVertex };
    static IndexBuffer* GenerateIndexBuffer(unsigned int count);
    static std::array<Vertex, 4> CreateQuad(float size, GLuint64 textureID);
    static std::array<Vertex, 4> CreateQuad(float width, float height, GLuint64 textureID);
    static bool IsQuadEmpty(std::array<Vertex, 4> quad);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PUBLIC/////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Renderer::OnEvent(Event& event) {
        EventType type = event.GetEventType();
        switch (type) {
        case EventType::APP_UPDATE:
            OnUpdate();
            break;
        case EventType::APP_SHUTDOWN:
            OnShutdown();
            break;
        case EventType::RENDER_NEW:
            ((RenderNewEvent&)event).m_QuadID = AddQuad(Global::PixelsPerUnit());
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
                GLuint64 slot = 0;
                if (((RenderUpdateEvent&)event).m_TexturePath != "") {
                    slot = AddTexture(((RenderUpdateEvent&)event).m_TexturePath);
                }
                SetQuadTextureID(((RenderUpdateEvent&)event).m_QuadID, slot);
                SetQuadColor(((RenderUpdateEvent&)event).m_QuadID, ((RenderUpdateEvent&)event).m_QuadColor);
                SetQuadTextureCoords(((RenderUpdateEvent&)event).m_QuadID, ((RenderUpdateEvent&)event).m_TexXCoords, ((RenderUpdateEvent&)event).m_TexYCoords);
                break;
            }
            break;
        }
        case EventType::RENDER_DELETE:
            RemoveQuad(((RenderDeleteEvent&)event).m_QuadID);
            break;
        case EventType::RENDER_NEW_TEXT: {
            auto newTextEvent = (RenderNewTextEvent&)event;
            auto font = m_Fonts[newTextEvent.m_FontPath];
            if (!font) {
                font = new Font(newTextEvent.m_FontPath, 100);
                m_Fonts.insert_or_assign(newTextEvent.m_FontPath, font);
            }
            ((RenderNewTextEvent&)event).m_QuadID = DrawText(*font, newTextEvent.m_Text);
            break;
        }
        case EventType::RENDER_UPDATE_TEXT_COLOR: {
            auto updateTextEvent = (RenderUpdateTextColorEvent&)event;
            SetQuadColor(updateTextEvent.m_QuadID, updateTextEvent.m_QuadColor);
            break;
        }
        case EventType::WINDOW_RESIZED: {
            auto resizeEvent = ((WindowResizedEvent&)event);
            Resize(resizeEvent.GetXDimension(), resizeEvent.GetYDimension());
            break;
        }
        case EventType::APP_START:
            OnStart(((ApplicationStartEvent&)event).GetWindowXDimension(), ((ApplicationStartEvent&)event).GetWindowYDimension());
            break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////PRIVATE////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    void Renderer::OnStart(float xDimension, float yDimension) {
        if (xDimension < 0 || yDimension < 0) {
            Global::FileLogger().Error("Renderer dimensions can not be negative.");
            exit(EXIT_FAILURE);
        }
        // Setup blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // Allow sorting by z position
        glEnable(GL_DEPTH_TEST);

        m_VertexBuffer = new VertexBuffer(nullptr, sizeof(Vertex) * 40);;
        m_IndexBuffer = GenerateIndexBuffer(60);
        m_VertexArray = new VertexArray();

        m_Shader = new Shader("shaders/Default.vert", "shaders/Default.frag");

        // Define the layout of the vertex buffer
        m_VertexArray->Bind();
        m_VertexBuffer->Bind();
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, Position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, Color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
        glEnableVertexAttribArray(3);
        glVertexAttribLPointer(3, 1, GL_UNSIGNED_INT64_ARB,
            sizeof(Vertex), (void*)offsetof(Vertex, TextureID));
        m_VertexBuffer->Unbind();
        m_VertexArray->Unbind();

        // By default, the projection matrix will have the center of the screen at (0,0) and
        // the the edges will be determined by the x and y values given 
        m_Shader->Bind();
        m_Shader->SetUniformMatrix4f("u_ProjectionMatrix", glm::ortho(-(float)xDimension / 2, (float)xDimension / 2, -(float)yDimension / 2, (float)yDimension / 2, -100.0f, 100.0f));
        m_Shader->Unbind();
    }

    void Renderer::OnUpdate() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Draw();
    }

    void Renderer::OnShutdown() {
        // TODO make sure everything is getting cleaned up
        delete m_VertexBuffer;
        delete m_VertexArray;
        delete m_IndexBuffer;
        delete m_Shader;
        // Deallocate all of the textures pointers
        for (Texture* t : m_BoundTextures) {
            delete t;
        }
        // Clear Vectors
        m_Quads.clear();
        m_CalculatedQuads.clear();
        m_ModelMatrices.clear();
        m_BoundTextures.clear();
        delete this;
    }

    const unsigned int Renderer::AddQuad(float size) {

        if (size <= 0) {
            Global::FileLogger().Error("A quad must have a positive, nonzero size.");
            exit(EXIT_FAILURE);
        }

        ValidateBuffers();

        auto quad = CreateQuad(size, 0);

        // If there are no gaps in the vector then just push to the back
        if (m_DeletedQuadIndexes.size() == 0) {
            m_Quads.push_back(quad);
            m_CalculatedQuads.push_back(quad);
            // Add a new model matrix to the back of the quads vector set to the identity matrix
            m_ModelMatrices.push_back(glm::mat4(1.0f));

            UpdateCalculatedQuads(m_Quads.size() - 1);
            // Return the index where the quad and model matrix are set
            return m_Quads.size() - 1;
        }

        // Find the next deleted Quad in the vector
        auto nextOpen = m_DeletedQuadIndexes.top();
        m_Quads.at(nextOpen) = quad;
        UpdateCalculatedQuads(nextOpen);
        m_DeletedQuadIndexes.pop();
        return nextOpen;

        return -1; // Some kind of error occured if this is reached
    }
    const unsigned int Renderer::AddQuad(float width, float height, unsigned long long textureID) {

        if (width <= 0 || height <= 0) {
            Global::FileLogger().Error("A quad must have a positive, nonzero size.");
            exit(EXIT_FAILURE);
        }

        ValidateBuffers();

        auto quad = CreateQuad(width, height, textureID);

        // If there are no gaps in the vector then just push to the back
        if (m_DeletedQuadIndexes.size() == 0) {
            m_Quads.push_back(quad);
            m_CalculatedQuads.push_back(quad);
            // Add a new model matrix to the back of the quads vector set to the identity matrix
            m_ModelMatrices.push_back(glm::mat4(1.0f));

            UpdateCalculatedQuads(m_Quads.size() - 1);
            // Return the index where the quad and model matrix are set
            return m_Quads.size() - 1;
        }

        // Find the next deleted Quad in the vector
        auto nextOpen = m_DeletedQuadIndexes.top();
        m_Quads.at(nextOpen) = quad;
        UpdateCalculatedQuads(nextOpen);
        m_DeletedQuadIndexes.pop();
        return nextOpen;

        return -1; // Some kind of error occured if this is reached
    }

    // FIXME deleted quads can still be modified by using the index in another function. It is not 
    // crucial to fix this immediately, but this will cause problems if the index of a deleted quad 
    // is used because the number of deleted quads will not equal the running count. This will 
    // eventually result in errors adding quads since a new quad may not be added and -1 will be 
    // returned. This can probably be solved quickly by checking an index against the indexes of 
    // deleted quads before performing any operations.

    const bool Renderer::RemoveQuad(const unsigned int quadIndex) {
        try {
            m_Quads.at(quadIndex) = ClearedQuad;
            m_CalculatedQuads.at(quadIndex) = ClearedQuad;
            m_ModelMatrices.at(quadIndex) = glm::translate(glm::mat4(1), glm::vec3(0, 0, 0));
            m_DeletedQuadIndexes.push(quadIndex);
            UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;

    }

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

    const unsigned int Renderer::DrawText(Font& font, const std::string& text) {
        unsigned int width, height;
        auto buffer = font.GetTextureBufferForText(text.c_str(), width, height);
        Texture* tex = new Texture(buffer.data(), width, height, 4);
        m_BoundTextures.push_back(tex);
        auto quadID = AddQuad(width, height, tex->GetHandle());
        SetQuadTextureCoords(quadID, { 1,1,0,0 }, { 0,1,1,0 });

        return quadID;
    }

    // Set the model matrix for the corresponding index, should have scale, rotation,
    // and transform already applied
    const bool Renderer::SetQuadModelMatrix(const unsigned int quadIndex, const glm::mat4 modelMatrix) {
        try {

            glm::mat4 scaledModelMatrix = modelMatrix;

            glm::vec3 position = modelMatrix[3];
            position *= Global::PixelsPerUnit();
            scaledModelMatrix[3] = glm::vec4(position, 1.0f);

            m_ModelMatrices[quadIndex] = scaledModelMatrix;
            UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    // Sets the color of the quad at the given index to the given color
    const bool Renderer::SetQuadColor(const unsigned int quadIndex, const glm::vec4 color) {
        try {
            auto quad = m_Quads.at(quadIndex);
            for (int i = 0; i < 4; i++) {
                quad.at(i).Color = color;
            }
            m_Quads.at(quadIndex) = quad;
            UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    // The coords should start in the top right corner and go clockwise
    const bool Renderer::SetQuadTextureCoords(const unsigned int quadIndex, const glm::vec4 xCoords, const glm::vec4 yCoords) {
        try {
            for (int i = 0; i < 4; i++) {
                m_Quads.at(quadIndex).at(i).TextureCoords = glm::vec2(xCoords[i], yCoords[i]);
            }
            UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    const bool Renderer::SetQuadTextureID(const unsigned int quadIndex, const unsigned long long textureID) {
        try {
            for (int i = 0; i < 4; i++) {
                m_Quads.at(quadIndex).at(i).TextureID = textureID;
            }
            UpdateCalculatedQuads(quadIndex);
        }
        catch (const std::exception& e) {
            return false;
        }
        return true;
    }

    void Renderer::SetViewMatrix(glm::mat4 viewMatrix) {

        glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
        glm::vec3 position = glm::vec3(inverseViewMatrix[3]);
        position *= Global::PixelsPerUnit();
        glm::vec3 forward = glm::vec3(-inverseViewMatrix[2]);
        glm::vec3 target = position + forward;
        glm::vec3 up = glm::vec3(inverseViewMatrix[1]);

        m_ViewMatrix = glm::lookAt(position, target, up);
    }

    const unsigned long long Renderer::AddTexture(const std::string& path) {

        for (Texture* t : m_BoundTextures) {
            if (path.compare(t->m_FilePath) == 0) {
                return t->m_Handle;
            }
        }

        Texture* texture = new Texture(path);
        m_BoundTextures.push_back(texture);
        return texture->GetHandle();
    }

    void Renderer::UpdateCalculatedQuads(const unsigned int index) {
        auto quad = m_Quads.at(index);
        auto modelMatrix = m_ModelMatrices.at(index);
        for (int j = 0; j < 4; j++) {
            quad[j].Position = modelMatrix * glm::vec4(quad.at(j).Position, 1.0f);
        }

        m_CalculatedQuads[index] = quad;

        // Update the GPU's data to reflect this
        m_VertexBuffer->Bind();
        glBufferSubData(GL_ARRAY_BUFFER, index * 4 * sizeof(Vertex), sizeof(Vertex) * 4, &m_CalculatedQuads[index]);
        m_VertexBuffer->Unbind();
    }

    void Renderer::ValidateBuffers() {

        // When we no longer have space in the buffers, double their size. If an issue arises where 
        // the buffers get really large and the number of quads then drops, this could be modified
        // to reduce the size when appropriate, but for now it is probably safer to leave it like this.
        // Realistically it is likely that the number of quads hits a high amount at one point it may 
        // happen again.
        if (m_IndexBuffer->GetCount() / 6 <= m_Quads.size()) {
            // Double the size of the vertex and index buffers
            unsigned int newVertexCount = m_IndexBuffer->GetCount() / 6 * 4 * 2;
            unsigned int newIndexCount = m_IndexBuffer->GetCount() / 6 * 6 * 2;

            VertexBuffer* newVB = new VertexBuffer(nullptr, sizeof(Vertex) * newVertexCount);
            newVB->Bind();
            // copy existing vertex data into new buffer
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                sizeof(Vertex) * m_CalculatedQuads.size() * 4,
                m_CalculatedQuads.data());
            newVB->Unbind();

            IndexBuffer* newIB = GenerateIndexBuffer(newIndexCount);

            // Rebind the VAO
            m_VertexArray->Bind();
            newVB->Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (void*)offsetof(Vertex, Position));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (void*)offsetof(Vertex, Color));
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                sizeof(Vertex), (void*)offsetof(Vertex, TextureCoords));
            glEnableVertexAttribArray(3);
            glVertexAttribLPointer(3, 1, GL_UNSIGNED_INT64_ARB,
                sizeof(Vertex), (void*)offsetof(Vertex, TextureID));
            newVB->Unbind();
            m_VertexArray->Unbind();

            // Swap in new buffers
            delete m_VertexBuffer;
            delete m_IndexBuffer;
            m_VertexBuffer = newVB;
            m_IndexBuffer = newIB;
        }

    }

    void Renderer::Resize(int xDimension, int yDimension) {
        m_Shader->Bind();
        m_Shader->SetUniformMatrix4f("u_ProjectionMatrix", glm::ortho(-(float)xDimension / 2, (float)xDimension / 2, -(float)yDimension / 2, (float)yDimension / 2, std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()));
        m_Shader->Unbind();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////NONCLASS///////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////

    static IndexBuffer* GenerateIndexBuffer(unsigned int count) {
        if (count < 6 || count % 6 != 0) {
            Global::FileLogger().Error("The number of verticies in an index buffer must be a positive multiple of 6.");
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

    static std::array<Vertex, 4> CreateQuad(float size, GLuint64 textureID) {

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

    static std::array<Vertex, 4> CreateQuad(float width, float height, GLuint64 textureID) {

        // Top Right
        Vertex v0;
        v0.Position = { width / 2, height / 2, 0.0f };
        v0.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v0.TextureCoords = { 1.0f, 1.0f };
        v0.TextureID = textureID;

        // Bottom Right
        Vertex v1;
        v1.Position = { width / 2, -height / 2, 0.0f };
        v1.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v1.TextureCoords = { 1.0f, 0.0f };
        v1.TextureID = textureID;

        // Bottom Left
        Vertex v2;
        v2.Position = { -width / 2, -height / 2, 0.0f };
        v2.Color = { 1.0f, 1.0f, 1.0f, 1.0f };
        v2.TextureCoords = { 0.0f, 0.0f };
        v2.TextureID = textureID;

        // Top Left
        Vertex v3;
        v3.Position = { -width / 2, height / 2, 0.0f };
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

}
