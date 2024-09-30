#include "Shader.h"
#include "../ThirdParty/glad/glad.h"

#include <iostream>
#include <fstream>
#include <sstream>

static unsigned int CompileShader(GLenum shaderType, const char* source) {
  // Create and compile the shader
  unsigned int shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Valide the shader compilation
  int result;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE) {
    int length;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*) alloca(length * sizeof(char));
    glGetShaderInfoLog(shader, length, &length, message);
    std::cout << "Failed to compile shader\n";
    std::cout << message<<"\n";
  }
  
  return shader;
}

// Parse the data from the given filepath to a string
static std::string Parse(const std::string& path) {
  std::ifstream ifStream(path);
  return std::string((std::istreambuf_iterator<char>(ifStream)),
                  (std::istreambuf_iterator<char>()));
}

GLint Shader::GetUniformLocation(const std::string& name) const {
  if(uniformLocationCache.find(name) != uniformLocationCache.end()) {
    return uniformLocationCache[name];
  }
  int location = glGetUniformLocation(programID, name.c_str());
  uniformLocationCache[name] = location;
  return location;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

  std::string vertexShader = Parse(vertexPath);
  std::string fragmentShader = Parse(fragmentPath);

  unsigned int program = glCreateProgram();

  // Compile Shaders
  unsigned int vShader = CompileShader(GL_VERTEX_SHADER, vertexShader.c_str());
  unsigned int fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader.c_str()); 

  // Attach the shaders to the program
  glAttachShader(program, vShader);
  glAttachShader(program, fShader);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vShader);
  glDeleteShader(fShader);

  programID = program;
}

Shader::~Shader() {
  glDeleteProgram(programID);
}

void Shader::Bind() const {
  glUseProgram(programID);
}

void Shader::Unbind() const {
  glUseProgram(0);
}

void Shader::SetUniform4f(const char* uniformName, float v1, float v2, float v3, float v4) {
  int location = GetUniformLocation(uniformName);
  glUniform4f(location, v1, v2, v3, v4);
}

void Shader::SetUniform1i(const char* uniformName, int i) {
  int location = GetUniformLocation(uniformName);
  glUniform1i(location, i);
}

void Shader::SetUniform1iv(const char* uniformName, int count, const int* values) {
  int location = GetUniformLocation(uniformName);
  glUniform1iv(location, count, values);
}

void Shader::SetUniformMatrix4f(const char* uniformName, const glm::mat4& matrix) {
  int location = GetUniformLocation(uniformName);
  glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetUniformMatrix4fv(const char* uniformName, const int count, const glm::mat4& values) {
   int location = GetUniformLocation(uniformName);
   glUniformMatrix4fv(location, count, GL_FALSE, &values[0][0]);
}