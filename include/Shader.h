#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "glm/glm.hpp"

//--------------------------------------------------------------------------------------------

class Shader
{
  public:
    Shader() = delete;

    explicit Shader(
      const std::string& vertexPath,
      const std::string& fragmentPath
    );

    ~Shader() = default;

    const Shader(const Shader&) = delete;

    void Use() const;

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

    void SetMat4Uniform(
      const std::string& name,
      glm::mat4 value
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

    unsigned int CompileShader(
      const char* shaderCode,
      ShaderTypes shaderType
    );

    void LinkShaders(
      unsigned int vertexShader,
      unsigned int fragmentShader
    );

    void CheckCompileErrors(
      unsigned int shader,
      CompileErrorTypes errorType
    ) const;

    unsigned int programID;
};

#endif