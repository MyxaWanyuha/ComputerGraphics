#include "Cone.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleEngine
{

const char* cone_vertex_shader = R"(
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

const char* cone_fragment_shader = R"(
#version 460
in vec3 color;
out vec4 frag_color;
void main() {
   frag_color = vec4(color, 1.0);
})";

Cone::Cone(const GLfloat height, const float r, const float cx, const float cy, const float cz, const u32 num_segments, const glm::vec3& color)
{
    m_color = color;
    generateCone(height, r, cx, cy, cz, num_segments);
    m_p_shader_program = std::make_unique<ShaderProgram>(cone_vertex_shader, cone_fragment_shader);
    rotationUniformLoc = m_p_shader_program->get_uniform_location("rotationMatrix");

    m_p_vao = std::make_unique<VertexArray>();

    m_p_positions_colors_vbo = std::make_unique<VertexBuffer>(
        positionsColors.data(),
        sizeof(positionsColors.data()[0]) * positionsColors.size(),
        buffer_layout_2_vec3);

    m_p_index_buffer = std::make_unique<IndexBuffer>(indices.data(), indices.size());

    m_p_vao->add_vertex_buffer(*m_p_positions_colors_vbo);
    m_p_vao->set_index_buffer(*m_p_index_buffer);
}

void Cone::rotate(glm::vec3 axis, float angle)
{
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

void Cone::render()
{
    m_p_shader_program->bind();
    m_p_vao->bind();
    glDrawElements(GL_LINES,
        static_cast<GLsizei>(m_p_vao->get_indices_count()),
        GL_UNSIGNED_INT,
        nullptr);
}

void Cone::generateCone(const GLfloat height, const float r, const float cx, const float cy, const float cz, const u32 num_segments)
{
    positionsColors.resize((num_segments + 2) * 6);
    indices.resize(num_segments * 6);

    positionsColors[0] = cx;
    positionsColors[1] = cy;
    positionsColors[2] = cz;
    positionsColors[3] = m_color[0];
    positionsColors[4] = m_color[1];
    positionsColors[5] = m_color[2];

    i32 index = 6;
    for (i32 i = 0; i < num_segments; ++i)
    {
        const float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(num_segments);//get the current angle
        const float x = r * cos(theta);
        const float y = r * sin(theta);
        const float z = 0.f;
        positionsColors[index++] = x + cx;
        positionsColors[index++] = y + cy;
        positionsColors[index++] = z + cz;
        positionsColors[index++] = m_color[0];
        positionsColors[index++] = m_color[1];
        positionsColors[index++] = m_color[2];
    }
    positionsColors[positionsColors.size() - 6] = cx;
    positionsColors[positionsColors.size() - 5] = cy;
    positionsColors[positionsColors.size() - 4] = cz + height;
    positionsColors[positionsColors.size() - 3] = m_color[0];
    positionsColors[positionsColors.size() - 2] = m_color[1];
    positionsColors[positionsColors.size() - 1] = m_color[2];

    i32 ii = 0;
    for (i32 i = 1; i < (num_segments + 1); ++i)
    {
        indices[ii] = 0;
        indices[ii + 1] = i;
        ii += 2;

        indices[ii] = positionsColors.size() / 6 - 1;
        indices[ii + 1] = i;
        ii += 2;
    }

    for (i32 i = 1; ii < indices.size() - 1; ii += 2)
    {
        indices[ii] = i;
        indices[ii + 1] = ++i;
    }
    indices[indices.size() - 1] = 1;
}

}