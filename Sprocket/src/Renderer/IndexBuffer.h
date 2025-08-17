#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

namespace Sprocket {


    /// @brief This is an abstraction for an OpenGL IndexBuffer.
    class IndexBuffer {
    private:
        unsigned int m_RendererID;
        unsigned int m_Count;
    public:

        /// @brief Creates a new IndexBuffer with the given indecies and the number of them.
        /// @param data The indecies that should be entered into the IndexBuffer.
        /// @param count The number of indecies that have been given.
        IndexBuffer(const unsigned int* data, unsigned int count);
        /// @brief Frees the OpenGL ID for this index buffer.
        ~IndexBuffer();

        /// @brief Binds the active IndexBuffer to being this.
        void Bind() const;
        /// @brief Unbinds this as the active IndexBuffer.
        void Unbind() const;
        /// @brief Gives the number of indicies in this IndexBuffer.
        /// @return The number of indicies.
        inline unsigned int GetCount() const { return m_Count; }
    };

}

#endif