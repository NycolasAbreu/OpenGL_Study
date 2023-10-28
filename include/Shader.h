#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
  public:
    Shader() = delete;

    explicit Shader(
      const std::string& vertexPath,
      const std::string& fragmentPath
    );

    const Shader(const Shader&) = delete;

    void Activate() const;

    void SetBoolUniform(
      const std::string& name,
      bool value)
      const;

    void SetIntUniform(
      const std::string& name,
      int value
    ) const;

    void SetFloatUniform(
      const std::string& name,
      float value
    ) const;

  private:
    enum class CompileErrorTypes
    {
      UNKNOW = 0,
      PROGRAM,
      VERTEX,
      FRAGMENT
    };

    enum class ShaderTypes
    {
      UNKNOW = 0,
      VERTEX,
      FRAGMENT
    };

    void ExtractTextFromFile(
      const std::string& vertexPath,
      const std::string& fragmentPath,
      std::string& vertexCode,
      std::string& fragmentCode
    ) const;

    unsigned int CompileShader(
      const char* shaderCode,
      ShaderTypes shaderType
    );

    void CheckCompileErrors(
      unsigned int shader,
      CompileErrorTypes errorType
    ) const;

    unsigned int programID;
};

#endif