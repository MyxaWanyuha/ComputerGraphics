#include "Model.hpp"
#include "ModelLoader.hpp"
#include "SimpleEngineCore/Log.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "SimpleEngineCore/stl_reader.hpp"
namespace SimpleEngine
{
	Model::Model(const char* path)
	{
        std::vector<float> coords, normals;
        std::vector<unsigned int> tris, solids;
        try {
            stl_reader::ReadStlFile(path,
                coords, normals, tris, solids);
        }
        catch (std::exception& e) {
            LOG_ERROR(e.what());
        }

        std::vector<float> positions_colors;
        for (u64 i = 0; i < coords.size(); i += 3)
        {
            positions_colors.push_back(coords[i]    );
            positions_colors.push_back(coords[i + 1]);
            positions_colors.push_back(coords[i + 2]);

            positions_colors.push_back(i * (1.0f / coords.size()));
            positions_colors.push_back(i * (1.0f / coords.size()));
            positions_colors.push_back(i * (1.0f / coords.size()));
        }

        std::vector<unsigned int> indices = tris;

        m_p_shader_program = std::make_unique<ShaderProgram>(default_vertex_shader, default_fragment_shader);
        model_matrix_uniform_loc = m_p_shader_program->get_uniform_location("model_matrix");

        m_p_vao = std::make_unique<VertexArray>();
        const BufferLayout buffer_layout_2_vec3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };
        m_p_positions_colors_vbo = std::make_unique<VertexBuffer>(
            positions_colors.data(),
            sizeof(positions_colors.data()[0]) * positions_colors.size(),
            buffer_layout_2_vec3,
            VertexBuffer::EUsage::Dynamic);

        m_p_index_buffer = std::make_unique<IndexBuffer>(
            indices.data(), 
            indices.size(), 
            VertexBuffer::EUsage::Dynamic);

        m_p_vao->add_vertex_buffer(*m_p_positions_colors_vbo);
        m_p_vao->set_index_buffer(*m_p_index_buffer);
	}

	void Model::render()
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
