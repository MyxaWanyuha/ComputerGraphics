#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <glad/glad.h>

namespace SimpleEngine
{

class Texture
{
public:
    Texture(const char* image,
            GLenum texType = GL_TEXTURE_2D,
            GLenum slot = GL_TEXTURE0,
            GLenum pixelType = GL_UNSIGNED_BYTE);

    Texture() = delete;
    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&& texture) = delete;
    Texture(Texture&& texture) = delete;
    ~Texture();

    void bind() const;
    void unbind() const;
private:
    GLuint m_ID;
    GLenum m_type;
};

}

#endif // TEXTURE_HPP