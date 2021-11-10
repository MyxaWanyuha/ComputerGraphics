#ifndef MODEL_HPP
#define MODEL_HPP

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

class Model : public Shape
{
public:
    Model(const char* path);
    virtual ~Model() override = default;
    virtual void render() override;
    const ShaderProgram& get_shader_program() const { return *m_p_shader_program; }
private:
    i32 model_matrix_uniform_loc;
    std::unique_ptr<ShaderProgram> m_p_shader_program;
    std::unique_ptr<VertexArray>   m_p_vao;
    std::unique_ptr<VertexBuffer>  m_p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer>   m_p_index_buffer;
};

}

#endif // MODEL_HPP