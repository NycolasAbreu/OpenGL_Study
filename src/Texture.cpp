#include "Texture.h"

#include <iostream>

#include <glad/glad.h>
#include <stb/stbimage.h>

//--------------------------------------------------------------------------------------------

Texture::Texture(
  const std::string& path,
  ImageExtension imageExt,
  bool shouldFlip
)
{
  // load and generate the texture
  int width = 0;
  int height = 0;
  int nrChannels = 0;

  if (shouldFlip) {
    stbi_set_flip_vertically_on_load(true);
  }

  unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
  if (data != nullptr) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    switch (imageExt) {
      case JPG: {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        break;
      }
      case PNG: {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        break;
      }
      default: {
        std::cout << "Unknow image extension: " << path << "\n";
        break;
      }
    }

    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Failed to load texture: " << path << "\n";
  }

  if (shouldFlip) {
    stbi_set_flip_vertically_on_load(false);
  }
  stbi_image_free(data);
}

//--------------------------------------------------------------------------------------------

void Texture::Use() const
{
  glBindTexture(GL_TEXTURE_2D, id);
}

//--------------------------------------------------------------------------------------------

void Texture::UseUnit(
  unsigned int textureUnitNum
) const 
{
  glActiveTexture(GL_TEXTURE0 + textureUnitNum);
  Use();
}

//--------------------------------------------------------------------------------------------
