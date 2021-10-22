#include "Trapezoid.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleEngine
{

const char* trapezoid_vertex_shader = R"(
#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
out vec3 color;
uniform mat4 camMatrix;
uniform mat4 rotationMatrix;
void main() {
    color = vertex_color;
    gl_Position = camMatrix * rotationMatrix * vec4(vertex_position, 1.0);
})";

const char* trapezoid_fragment_shader = R"(
#version 460
in vec3 color;
out vec4 frag_color;
void main() {
   frag_color = vec4(color, 1.0);
})";


Trapezoid::Trapezoid(const GLfloat height, const float up_length, const float down_length, const glm::vec3& color)
{
    m_color = color;
    generate(height, up_length, down_length, cx, cy, cz);
    m_p_shader_program = std::make_unique<ShaderProgram>(trapezoid_vertex_shader, trapezoid_fragment_shader);
    rotationUniformLoc = m_p_shader_program->get_uniform_location("rotationMatrix");

    m_p_vao = std::make_unique<VertexArray>();

    m_p_positions_colors_vbo = std::make_unique<VertexBuffer>(
        positionsColors.data(),
        sizeof(positionsColors.data()[0]) * positionsColors.size(),
        buffer_layout_2_vec3,
        VertexBuffer::EUsage::Dynamic);

    m_p_index_buffer = std::make_unique<IndexBuffer>(indices.data(), indices.size(), VertexBuffer::EUsage::Dynamic);

    m_p_vao->add_vertex_buffer(*m_p_positions_colors_vbo);
    m_p_vao->set_index_buffer(*m_p_index_buffer);
}

void Trapezoid::rotate(glm::vec3 axis, float angle)
{
    m_p_vao->enable_vertex_buffer();
    m_p_vao->set_index_buffer(*m_p_index_buffer);
    rotAngle += angle;
    m_p_shader_program->bind();
    glUniformMatrix4fv(rotationUniformLoc, 1, GL_FALSE,
        glm::value_ptr(glm::rotate(rotAngle, axis)));
    m_p_vao->bind();
    glDrawElements(GL_LINES,
        static_cast<GLsizei>(m_p_vao->get_indices_count()),
        GL_UNSIGNED_INT,
        nullptr);
}

void Trapezoid::render()
{
    m_p_shader_program->bind();
    m_p_vao->bind();
    glDrawElements(GL_LINES,
        static_cast<GLsizei>(m_p_vao->get_indices_count()),
        GL_UNSIGNED_INT,
        nullptr);
}

void Trapezoid::generate(const GLfloat height, const float up_length, const float down_length)
{
    positionsColors.resize(24);
    indices.resize(24);

    //positionsColors[0]
    //positionsColors[0]
    //positionsColors[0]


}

}