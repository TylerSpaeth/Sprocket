#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include "../ThirdParty/glm/glm.hpp"

#include "../Macros.h"

namespace Sprocket {

  class SPROCKET_API Shader {
    private:
      unsigned int programID;
      mutable std::unordered_map<std::string, int> uniformLocationCache;
      int GetUniformLocation(const std::string& name) const;
    public:
      Shader(const std::string& vertexPath, const std::string& fragmentPath);
      ~Shader();
      void Bind() const;
      void Unbind() const;
      void SetUniform4f(const char* uniformName, float v1, float v2, float v3, float v4);
      void SetUniform1i(const char* uniformName, int i);
      void SetUniform1iv(const char* uniformName, int count, const int* values);
      void SetUniformMatrix4f(const char* uniformName, const glm::mat4& matrix);
      void SetUniformMatrix4fv(const char* uniformName, const int count, const glm::mat4& values);
  };

}

#endif