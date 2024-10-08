#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include "VertexBuffer.h"

namespace Sprocket {

  /// @brief Encapsulates OpenGL vertex array functionality.
  class VertexArray {
    private:
      unsigned int rendererID;
    public:

      /// @brief Creates a new vertex array.
      VertexArray();

      /// @brief Frees the OpenGL array ID.
      ~VertexArray();

      /// @brief Makes this array active.
      void Bind() const;

      /// @brief Makes the active array index 0.
      void Unbind() const;
  };

}

#endif