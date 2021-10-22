#include "Cylinder.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleEngine
{

const char* cylinder_vertex_shader = R"(
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

const char* cylinder_fragment_shader = R"(
#version 460
in vec3 color;
out vec4 frag_color;
void main() {
   frag_color = vec4(color, 1.0);
})";

Cylinder::Cylinder(const GLfloat height, const float r, const float cx, const float cy, const float cz, const u32 num_segments, const glm::vec3& color)
{
    m_color = color;
    generate(height, r, cx, cy, cz, num_segments);
    m_p_shader_program = std::make_unique<ShaderProgram>(cylinder_vertex_shader, cylinder_fragment_shader);
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

void Cylinder::rotate_render(glm::vec3 axis, float angle)
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

void Cylinder::render()
{
    m_p_shader_program->bind();
    m_p_vao->bind();
    glDrawElements(GL_LINES,
        static_cast<GLsizei>(m_p_vao->get_indices_count()),
        GL_UNSIGNED_INT,
        nullptr);
}

void Cylinder::generate(const GLfloat height, const float r, const float cx, const float cy, const float cz, const u32 num_segments_dots)
{
    const i32 centers_count = 2;
    const i32 circles_count = 2;
    const i32 numbers_for_one_point = 6;// 3 for xyz && 3 for color
    positionsColors.resize((num_segments_dots * circles_count + centers_count) * numbers_for_one_point);
    indices.resize(num_segments_dots * numbers_for_one_point);
    // center1 dot1_circle1, dot2_circle2 ... dotN_circle1, dotN_circle2 center2
    //center 1
    {
    positionsColors[0] = cx;
    positionsColors[1] = cy;
    positionsColors[2] = cz;
    positionsColors[3] = m_color[0];
    positionsColors[4] = m_color[1];
    positionsColors[5] = m_color[2];
    }
    i32 index = numbers_for_one_point;
    for (i32 i = 0; i < num_segments_dots; ++i)
    {
        const float theta = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(num_segments_dots);//get the current angle
        const float x = r * cos(theta);
        const float y = r * sin(theta);
        const float z = 0.f;
        positionsColors[index++] = x + cx;
        positionsColors[index++] = y + cy;
        positionsColors[index++] = z + cz;
        positionsColors[index++] = m_color[0];
        positionsColors[index++] = m_color[1];
        positionsColors[index++] = m_color[2];

        positionsColors[index++] = x + cx;
        positionsColors[index++] = y + cy;
        positionsColors[index++] = z + cz + height;
        positionsColors[index++] = m_color[0];
        positionsColors[index++] = m_color[1];
        positionsColors[index++] = m_color[2];
    }
    //center 2
    {
    positionsColors[positionsColors.size() - 6] = cx;
    positionsColors[positionsColors.size() - 5] = cy;
    positionsColors[positionsColors.size() - 4] = cz + height;
    positionsColors[positionsColors.size() - 3] = m_color[0];
    positionsColors[positionsColors.size() - 2] = m_color[1];
    positionsColors[positionsColors.size() - 1] = m_color[2];
    }


    const i32 center_first_index = 0;
    const i32 center_second_index = positionsColors.size() / numbers_for_one_point - 1;
    i32 segment_i = 1;
    for (i32 i = 0; i < num_segments_dots * numbers_for_one_point; i += numbers_for_one_point, segment_i += 2)
    {
        indices[i] = center_first_index;
        indices[i + 1] = segment_i;

        indices[i + 2] = segment_i;
        indices[i + 3] = segment_i + 1;

        indices[i + 4] = segment_i + 1;
        indices[i + 5] = center_second_index;
    }

    segment_i = 1;
    for (i32 i = 1; i < num_segments_dots * circles_count * 2; i += 4, segment_i += 2)
    {
        indices.push_back(segment_i);
        indices.push_back(segment_i + 2);

        indices.push_back(segment_i + 1);
        indices.push_back(segment_i + 3);
    }
    indices[indices.size() - 4] = 1;
    indices[indices.size() - 3] = positionsColors.size() / 6 - 3;
    indices[indices.size() - 2] = 2;
    indices[indices.size() - 1] = positionsColors.size() / 6 - 2;
}

}