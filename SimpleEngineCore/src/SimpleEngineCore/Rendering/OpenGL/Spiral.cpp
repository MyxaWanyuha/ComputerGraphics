#include "Spiral.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleEngine
{
const char* spiral_vertex_shader = R"(
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

const char* spiral_fragment_shader = R"(
#version 460
in vec3 color;
out vec4 frag_color;
void main() {
   frag_color = vec4(color, 1.0);
})";

Spiral::Spiral(const glm::vec3& color, const glm::vec3& center)
{
    generate(color, center);
    m_p_shader_program = std::make_unique<ShaderProgram>(spiral_vertex_shader, spiral_fragment_shader);
    rotationUniformLoc = m_p_shader_program->get_uniform_location("rotationMatrix");

    m_p_vao = std::make_unique<VertexArray>();

    const BufferLayout buffer_layout_2_vec3
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };
    m_p_positions_colors_vbo = std::make_unique<VertexBuffer>(
        positionsColors.data(),
        sizeof(positionsColors.data()[0]) * positionsColors.size(),
        buffer_layout_2_vec3,
        VertexBuffer::EUsage::Dynamic);

    m_p_index_buffer = std::make_unique<IndexBuffer>(indices.data(), indices.size(), VertexBuffer::EUsage::Dynamic);

    m_p_vao->add_vertex_buffer(*m_p_positions_colors_vbo);
    m_p_vao->set_index_buffer(*m_p_index_buffer);
}

void Spiral::rotate(glm::vec3 axis, float angle)
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

void Spiral::generate(const glm::vec3& color, const glm::vec3& center)
{
    const float rd = 0.35355534f;
    float px = 0;
    float py = 0;
    float z = 0.0f;
    positionsColors.push_back(center.x);
    positionsColors.push_back(center.y);
    positionsColors.push_back(center.z);
    positionsColors.push_back(0.5f);
    positionsColors.push_back(0.5f);
    positionsColors.push_back(0.5f);

    for (i32 i = 0; i < 360 * 6; i++)
    {
        const float t = i * M_PI / 180.0f;
        const float b = 2 * t;
        const float x = cos(b);
        const float y = sin(b);
        positionsColors.push_back(center.x + x);
        positionsColors.push_back(center.z + z);
        positionsColors.push_back(center.y + y);
        positionsColors.push_back(0.5f + x);
        positionsColors.push_back(0.5f + y);
        positionsColors.push_back(0.5f + z);
        z = static_cast<float>(i) / 360.0f;
    }

    for (i32 i = 0; i < 360 * 6 - 1; ++i)
    {
        indices.push_back(i + 1);
        indices.push_back(i + 2);
    }
}

}