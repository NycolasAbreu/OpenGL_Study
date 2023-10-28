#include "Shader.h"

//--------------------------------------------------------------------------------------------

Shader::Shader(
  const std::string& vertexPath,
  const std::string& fragmentPath
)
{
  std::string vertexCode;
  std::string fragmentCode;

  try {
    ExtractTextFromFile(vertexPath, fragmentPath, vertexCode, fragmentCode);
  }
  catch (std::ifstream::failure& e) {
    std::cout << "(Shader::Shader) Error Reading Shaders File: " << e.what() << std::endl;
  }

  // 2. compile shaders
  unsigned int vertex = CompileShader(vertexCode.c_str(), ShaderTypes::VERTEX);
  unsigned int fragment = CompileShader(fragmentCode.c_str(), ShaderTypes::FRAGMENT);

  // shader Program
  programID = glCreateProgram();
  glAttachShader(programID, vertex);
  glAttachShader(programID, fragment);
  glLinkProgram(programID);
  CheckCompileErrors(programID, CompileErrorTypes::PROGRAM);

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

//--------------------------------------------------------------------------------------------

void Shader::ExtractTextFromFile(
  const std::string& vertexPath,
  const std::string& fragmentPath,
  std::string& vertexCode,
  std::string& fragmentCode
) const
{
  std::ifstream vertexShaderFile;
  std::ifstream fraShaderFile;

  // ensure ifstream objects can throw exceptions:
  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fraShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  // open files
  vertexShaderFile.open(vertexPath);
  fraShaderFile.open(fragmentPath);

  // read file's buffer contents into streams
  std::stringstream vShaderStream;
  std::stringstream fShaderStream;
  vShaderStream << vertexShaderFile.rdbuf();
  fShaderStream << fraShaderFile.rdbuf();

  // return code from stream
  vertexCode = vShaderStream.str();
  fragmentCode = fShaderStream.str();

  // close file handlers
  vertexShaderFile.close();
  fraShaderFile.close();
}

//--------------------------------------------------------------------------------------------

unsigned int Shader::CompileShader(
  const char* shaderCode,
  ShaderTypes shaderType
)
{
  unsigned int shaderID = -1;
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

void Shader::Activate() const
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
    case CompileErrorTypes::UNKNOW:
    default:
      {
        std::cout << "UKNOW ERROR: " << "\n ------------------------------------------------------- \n";
        break;
      }
  }
}