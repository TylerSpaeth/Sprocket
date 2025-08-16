#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

namespace Sprocket {

  /// @brief Encapsulates OpenGL vertex buffer functionality.
  class VertexBuffer {
    private:
      unsigned int m_RendererID;
    public:

      /// @brief Creates an OpenGL vertex buffer of a given size with the given data.
      /// @param data the data that should be initialized in the vertex buffer.
      /// @param size the size of the vertex buffer.
      VertexBuffer(const void* data, unsigned int size);

      /// @brief Frees the OpenGL buffer ID.
      ~VertexBuffer();

      /// @brief Makes this buffer active.
      void Bind() const;

      /// @brief Makes the active buffer index 0.
      void Unbind() const;
  };

}

#endif