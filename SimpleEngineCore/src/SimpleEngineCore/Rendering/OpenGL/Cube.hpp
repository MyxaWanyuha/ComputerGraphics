#ifndef CUBE_HPP
#define CUBE_HPP
#include "Shape.hpp"

#include "SimpleEngineCore/Types.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"

#include <memory>
#include <glad/glad.h>

namespace SimpleEngine
{

class Cube : public Shape
{
public:
    Cube();
    virtual ~Cube() override = default;
    virtual void render() override;
    const ShaderProgram& get_shader_program() const { return *m_p_shader_program; }

private:
    static constexpr GLfloat positions_colors[] = {
  -1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f,
   1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f,
   1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f,
  -1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f,
  -1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 1.0f,
   1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 1.0f,
   1.0f,  1.0f, -1.0f,    1.0f, 1.0f, 1.0f,
  -1.0f,  1.0f, -1.0f,    1.0f, 1.0f, 1.0f };
    
    static constexpr GLuint indices[] = {
    0,1,
    1,2,
    2,3,
    3,0,
    4,5,
    5,6,
    6,7,
    7,4,
    0,4,
    1,5,
    2,6,
    3,7};

    i32 model_matrix_uniform_loc;
    std::unique_ptr<ShaderProgram> m_p_shader_program;
    std::unique_ptr<VertexArray>   m_p_vao;
    std::unique_ptr<VertexBuffer>  m_p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer>   m_p_index_buffer;
};

class TriangleCube : public Shape
{
public:
    TriangleCube();
    virtual ~TriangleCube() override = default;
    virtual void render() override;
    const ShaderProgram& get_shader_program() const { return *m_p_shader_program; }

private:
    static constexpr GLfloat positions_colors[] = {
   1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
   1.0f, -1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
   1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,
   1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 1.0f,
  -1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 1.0f,
  -1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 0.0f,
  -1.0f,  1.0f, -1.0f,    0.5f, 0.7f, 0.9f,
  -1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f };

    static constexpr GLuint indices[] = {
    4,0,3,
    4,3,7,
    0,1,2,
    0,2,3,
    1,5,6,
    1,6,2,
    5,4,7,
    5,7,6,
    7,3,2,
    7,2,6,
    0,5,1,
    0,4,5
    };

    i32 model_matrix_uniform_loc;
    std::unique_ptr<ShaderProgram> m_p_shader_program;
    std::unique_ptr<VertexArray>   m_p_vao;
    std::unique_ptr<VertexBuffer>  m_p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer>   m_p_index_buffer;
};

}

#endif // CUBE_HPP
