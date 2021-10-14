#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP
//#include "SimpleEngineCore/Types.hpp"
#include "../../../../../SimpleEngineCore/includes/SimpleEngineCore/Types.hpp"

namespace SimpleEngine {

class ShaderProgram
{
public:

    ShaderProgram(const char* vertex_shader_src, const char* fragment_shader_src);
    ~ShaderProgram();
    ShaderProgram(ShaderProgram&&) noexcept;
    ShaderProgram& operator=(ShaderProgram&&) noexcept;

    void bind() const;
    static void unbind();
    bool isCompiled() const { return m_isCompiled; }

    ShaderProgram() = delete;
    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram& operator=(const ShaderProgram&) = delete;

    i32 get_uniform_location(const char* uniform) const;

private:
    bool m_isCompiled = false;
    u32 m_id = 0;
};

}

#endif // SHADER_PROGRAM_HPP
