#include "Cube.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace SimpleEngine
{

Cube::Cube()
{
    m_p_shader_program = std::make_unique<ShaderProgram>(default_vertex_shader, default_fragment_shader);
    model_matrix_uniform_loc = m_p_shader_program->get_uniform_location("model_matrix");

    m_p_vao = std::make_unique<VertexArray>();
    const BufferLayout buffer_layout_2_vec3
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };
    m_p_positions_colors_vbo = std::make_unique<VertexBuffer>(
        positions_colors,
        sizeof(positions_colors),
        buffer_layout_2_vec3,
        VertexBuffer::EUsage::Dynamic);

    m_p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]), VertexBuffer::EUsage::Dynamic);

    m_p_vao->add_vertex_buffer(*m_p_positions_colors_vbo);
    m_p_vao->set_index_buffer(*m_p_index_buffer);
}

void Cube::render()
{
    m_p_vao->enable_vertex_buffer();
    m_p_vao->set_index_buffer(*m_p_index_buffer);
    m_p_shader_program->bind();

    glUniformMatrix4fv(model_matrix_uniform_loc, 1, GL_FALSE,
        glm::value_ptr(model_matrix));

    m_p_vao->bind();

    glDrawElements(GL_LINES,
        static_cast<GLsizei>(m_p_vao->get_indices_count()),
        GL_UNSIGNED_INT,
        nullptr);
}

TriangleCube::TriangleCube()
{
    m_p_shader_program = std::make_unique<ShaderProgram>(default_vertex_shader, default_fragment_shader);
    model_matrix_uniform_loc = m_p_shader_program->get_uniform_location("model_matrix");

    m_p_vao = std::make_unique<VertexArray>();
    const BufferLayout buffer_layout_2_vec3
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };
    m_p_positions_colors_vbo = std::make_unique<VertexBuffer>(
        positions_colors,
        sizeof(positions_colors),
        buffer_layout_2_vec3,
        VertexBuffer::EUsage::Dynamic);

    m_p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(indices[0]), VertexBuffer::EUsage::Dynamic);

    m_p_vao->add_vertex_buffer(*m_p_positions_colors_vbo);
    m_p_vao->set_index_buffer(*m_p_index_buffer);
}

void TriangleCube::render()
{
    m_p_vao->enable_vertex_buffer();
    m_p_vao->set_index_buffer(*m_p_index_buffer);
    m_p_shader_program->bind();

    glUniformMatrix4fv(model_matrix_uniform_loc, 1, GL_FALSE,
        glm::value_ptr(model_matrix));

    m_p_vao->bind();

    glDrawElements(GL_TRIANGLES,
        static_cast<GLsizei>(m_p_vao->get_indices_count()),
        GL_UNSIGNED_INT,
        nullptr);
}

}
