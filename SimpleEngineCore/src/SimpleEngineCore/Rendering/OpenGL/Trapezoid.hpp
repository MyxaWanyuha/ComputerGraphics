#ifndef TRAPEZOID_HPP
#define TRAPEZOID_HPP

#include "SimpleEngineCore/Types.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"

#include <memory>
#include <array>
#include <glad/glad.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace SimpleEngine
{

class Trapezoid
{
public:
    Trapezoid(const GLfloat height = 1.f, const float up_length = 0.5f, const float down_length = 1.0f, const glm::vec3& color = { 1, 1, 1 }, const glm::vec3& center = { 0,0,0 });
    void rotate(glm::vec3 axis = { 1.f,0.f,0.f }, float angle = 0.01f);
    void render();
    const VertexArray& getVertexArray() const { return *m_p_vao; }
    const ShaderProgram& getShaderProgram() const { return *m_p_shader_program; }

private:
    float rotAngle = 0;
    void generate(const GLfloat height, const float up_length, const float down_length, const glm::vec3& color, const glm::vec3& center);
    std::array<GLfloat, 8 * 6> positionsColors;
    std::array<GLuint, 24> indices;
    i32 rotationUniformLoc;
    std::unique_ptr<ShaderProgram> m_p_shader_program;
    std::unique_ptr<VertexArray> m_p_vao;
    std::unique_ptr<VertexBuffer> m_p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer> m_p_index_buffer;

    const BufferLayout buffer_layout_2_vec3
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };
};

}

#endif // TRAPEZOID_HPP
