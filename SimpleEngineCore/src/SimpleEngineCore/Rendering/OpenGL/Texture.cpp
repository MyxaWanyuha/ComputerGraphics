#include "Texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "../SimpleEngineCore/src/SimpleEngineCore/stb_image.h"
namespace SimpleEngine
{

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum pixelType)
{
    m_type = texType;
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    glGenTextures(1, &m_ID);
    glActiveTexture(slot);
    glBindTexture(texType, m_ID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum mode = numColCh == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(texType, 0, mode, widthImg, heightImg, 0, mode, pixelType, bytes);
    glGenerateMipmap(texType);
    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

void Texture::bind() const
{
    glBindTexture(m_type, m_ID);
}

void Texture::unbind() const
{
    glBindTexture(m_type, 0);
}

}