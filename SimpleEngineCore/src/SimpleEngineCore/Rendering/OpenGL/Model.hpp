#ifndef MODEL_HPP
#define MODEL_HPP

#include "Shape.hpp"

#include "SimpleEngineCore/Types.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Texture.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Material.hpp"

#include <memory>
#include <glad/glad.h>

namespace SimpleEngine
{

class Model : public Shape
{
public:
    Model(const char* path, const char* texture);
    Model(const char* stl_path);
    virtual ~Model() override = default;
    virtual void render() override;
    const ShaderProgram& get_shader_program() const { return *m_p_shader_program; }

    Model& operator=(const Model&) = delete;
    Model& operator=(Model&&) = delete;
private:
    Material material;
    i32 model_matrix_uniform_loc;
    std::unique_ptr<ShaderProgram> m_p_shader_program;
    std::unique_ptr<VertexArray>   m_p_vao;
    std::unique_ptr<VertexBuffer>  m_p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer>   m_p_index_buffer;
    std::unique_ptr<Texture>       m_p_texture;
    u64 vertex_count = 0;
    i32 tex0_loc;
};

}

#endif // MODEL_HPP