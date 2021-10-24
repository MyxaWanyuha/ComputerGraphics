#include "Torus.hpp"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleEngine
{
const char* torus_vertex_shader = R"(
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

const char* torus_fragment_shader = R"(
#version 460
in vec3 color;
out vec4 frag_color;
void main() {
   frag_color = vec4(color, 1.0);
})";

Torus::Torus(const float main_radius, const u32 main_segments, const float circle_radius, const float circle_segments, const glm::vec3& color, const glm::vec3& center)
{
    generate(main_radius, main_segments, circle_radius, circle_segments, color, center);
    m_p_shader_program = std::make_unique<ShaderProgram>(torus_vertex_shader, torus_fragment_shader);
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

void Torus::generate(const float main_radius, const u32 main_segments, const float circle_radius, const u32 circle_segments, const glm::vec3& color, const glm::vec3& center)
{
    float mainSegmentAngleStep = glm::radians(360.0f / float(main_segments));
    float tubeSegmentAngleStep = glm::radians(360.0f / float(circle_segments));

    auto currentMainSegmentAngle = 0.0f;
    for (i32 i = 0; i < main_segments; ++i)
    {
        float sinMainSegment = sin(currentMainSegmentAngle);
        float cosMainSegment = cos(currentMainSegmentAngle);
        float currentTubeSegmentAngle = 0.0f;
        for (i32 j = 0; j < circle_segments; ++j)
        {
            float sinTubeSegment = sin(currentTubeSegmentAngle);
            float cosTubeSegment = cos(currentTubeSegmentAngle);

            positionsColors.push_back((main_radius + circle_radius * cosTubeSegment) * cosMainSegment + center.x);
            positionsColors.push_back((main_radius + circle_radius * cosTubeSegment) * sinMainSegment + center.y);
            positionsColors.push_back(circle_radius * sinTubeSegment + center.z);
            positionsColors.push_back(color[0]);
            positionsColors.push_back(color[1]);
            positionsColors.push_back(color[2]);

            currentTubeSegmentAngle += tubeSegmentAngleStep;

            u32 j2 = i * circle_segments;
            for (u32 i2 = j2; i2 < (j2 + circle_segments - 1); i2 += 1)
            {
                indices.push_back(i2);
                indices.push_back(i2 + 1);
            }
            indices.push_back(j2);
            indices.push_back(j2 + circle_segments - 1);
        }
        currentMainSegmentAngle += mainSegmentAngleStep;
    }

    for (i32 i = 0; i < main_segments; ++i)
    {
        for (i32 j = 1; j < circle_segments; ++j)
        {
            indices.push_back(i + circle_segments * (j - 1));
            indices.push_back(i + circle_segments * j);
        }
        indices.push_back(i);
        indices.push_back(i + (main_segments - 1) * circle_segments);
    }
}

void Torus::rotate(glm::vec3 axis, float angle)
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

}