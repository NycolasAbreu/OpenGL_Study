#include "Shader.h"

#include <iostream>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.h"

//--------------------------------------------------------------------------------------------

Shader::Shader(
  const std::string& vertexPath,
  const std::string& fragmentPath
)
{
  std::string vertexCode;
  std::string fragmentCode;

  try {
    ExtractTextFromFile(vertexPath, vertexCode);
    ExtractTextFromFile(fragmentPath, fragmentCode);
  }
  catch (std::ifstream::failure& e) {
    std::cout << "(Shader::Shader) Error Reading Shaders File: " << e.what() << std::endl;
  }

  // compile shaders
  unsigned int vertex = CompileShader(vertexCode.c_str(), ShaderTypes::VERTEX);
  unsigned int fragment = CompileShader(fragmentCode.c_str(), ShaderTypes::FRAGMENT);

  // Link shaders and create program
  LinkShaders(vertex, fragment);

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

//--------------------------------------------------------------------------------------------

unsigned int Shader::CompileShader(
  const char* shaderCode,
  ShaderTypes shaderType
)
{
  unsigned int shaderID = -1U;
  if (shaderType == ShaderTypes::VERTEX) {
    shaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderID, 1, &shaderCode, nullptr);
    glCompileShader(shaderID);
    CheckCompileErrors(shaderID, CompileErrorTypes::VERTEX);
  }
  else if (shaderType == ShaderTypes::FRAGMENT) {
    shaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderID, 1, &shaderCode, nullptr);
    glCompileShader(shaderID);
    CheckCompileErrors(shaderID, CompileErrorTypes::FRAGMENT);
  }

  return shaderID;
}

//--------------------------------------------------------------------------------------------

void Shader::LinkShaders(
  unsigned int vertexShader,
  unsigned int fragmentShader
) 
{
  programID = glCreateProgram();
  glAttachShader(programID, vertexShader);
  glAttachShader(programID, fragmentShader);
  glLinkProgram(programID);
  CheckCompileErrors(programID, CompileErrorTypes::PROGRAM);
}

//--------------------------------------------------------------------------------------------

void Shader::Use() const
{
  glUseProgram(programID);
}

//--------------------------------------------------------------------------------------------

void Shader::SetBoolUniform(
  const std::string& name,
  bool value
) const
{
  glUniform1i(glGetUniformLocation(programID, name.c_str()), (int)value);
}

//--------------------------------------------------------------------------------------------

void Shader::SetIntUniform(
  const std::string& name,
  int value
) const
{
  glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

//--------------------------------------------------------------------------------------------

void Shader::SetFloatUniform(
  const std::string& name,
  float value
) const
{
  glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

//--------------------------------------------------------------------------------------------

void Shader::SetMat4Uniform(
  const std::string& name,
  glm::mat4 value
) const
{
  glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

//--------------------------------------------------------------------------------------------

void Shader::SetVec3Uniform(
  const std::string& name,
  glm::vec3 value
) const
{
  glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}

//--------------------------------------------------------------------------------------------

void Shader::CheckCompileErrors(
  unsigned int shader,
  CompileErrorTypes errorType
) const
{
  int success;
  char infoLog[1024];

  switch (errorType) {
    case CompileErrorTypes::VERTEX:
    case CompileErrorTypes::FRAGMENT:
    {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: CompileErrorTypes::" << (int)errorType << "\n"
          << infoLog << "\n ------------------------------------------------------- \n";
      }
      break;
    }
    case CompileErrorTypes::PROGRAM:
    {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success) {
        glGetProgramInfoLog(shader, 1024, nullptr, infoLog);

        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: CompileErrorTypes::" << (int)errorType << "\n"
          << infoLog << "\n ------------------------------------------------------- \n";
      }
      break;
    }
    default:
      {
        std::cout << "UKNOW ERROR " << "\n ------------------------------------------------------- \n";
        break;
      }
  }
}