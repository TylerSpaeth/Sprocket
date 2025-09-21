#ifndef SHADER_H
#define SHADER_H

#include "ThirdParty/glm/glm.hpp"
#include "Core/Sprocket.pch"

namespace Sprocket {

    /// @brief This acts as an abstraction for OpenGL shader functionality.
    class Shader {
    private:

        unsigned int m_ProgramID;
        mutable std::unordered_map<std::string, int> m_UniformLocationCache;

    public:

        /// @brief Creates a new OpenGL shader program based off of the two given files.
        /// @param vertexPath the filepath to a vertex shader.
        /// @param fragmentPath the filepath to a fragment shader.
        Shader(const std::string& vertexPath, const std::string& fragmentPath);

        /// @brief Frees the OpenGL shader ID.
        ~Shader();

        /// @brief Sets OpenGL to use this Shader's shader program.
        void Bind() const;

        /// @brief Sets the OpenGL shader program to 0.
        void Unbind() const;

        /// @brief Sets the vec4 uniform of the given name to have the given values.
        /// @param uniformName The name of uniform in the shader.
        /// @param v1 First value in the vector.
        /// @param v2 Second value in the vector.
        /// @param v3 Third value in the vector.
        /// @param v4 Fourth value in the vector.
        void SetUniform4f(const char* uniformName, float v1, float v2, float v3, float v4);

        /// @brief Sets the integer uniform to have the given value.
        /// @param uniformName The name of the uniform in the shader.
        /// @param i The integer to be set.
        void SetUniform1i(const char* uniformName, int i);

        /// @brief Sets a vector of integer uniforms to the given values.
        /// @param uniformName The name of the uniform in the shader.
        /// @param count The number of integers that are in values.
        /// @param values An array of integers to be stored in the uniform.
        void SetUniform1iv(const char* uniformName, int count, const int* values);

        /// @brief Sets the value of a single 4x4 float matrix uniform.
        /// @param uniformName The name of the uniform in the shader.
        /// @param matrix The matrix that should be stored.
        void SetUniformMatrix4f(const char* uniformName, const glm::mat4& matrix);

        /// @brief Stores an array of 4x4 float matricies in a unifrom.
        /// @param uniformName The name of the uniform in the shader.
        /// @param count The number of matricies in values.
        /// @param values The array of matricies that should be stored.
        void SetUniformMatrix4fv(const char* uniformName, const int count, const glm::mat4& values);

    private:

        int GetUniformLocation(const std::string& name) const;

    };

}

#endif