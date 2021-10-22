#ifndef CONE_HPP
#define CONE_HPP

#include "SimpleEngineCore/Types.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"

#include <memory>
#include <vector>
#include <glad/glad.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace SimpleEngine
{

class Cone
{
public:
    Cone(const GLfloat height = 1.f, const float r = 0.5f, const float cx = 0, const float cy = 0, const float cz = 0, const u32 num_segments = 30, const glm::vec3& color = {1, 1, 1});
    void rotate(glm::vec3 axis = { 1.f,0.f,0.f }, float angle = 0.01f);
    void render();
    const VertexArray& getVertexArray() const { return *m_p_vao; }
    const ShaderProgram& getShaderProgram() const { return *m_p_shader_program; }

private:
    float rotAngle = 0;
    void generate(const GLfloat height, const float r, const float cx, const float cy, const float cz, const u32 num_segments);
    std::vector<GLfloat> positionsColors;
    std::vector<GLuint> indices;
    i32 rotationUniformLoc;
    glm::vec3 m_color;
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

#endif // CONE_HPP
