#include "Model.hpp"
#include "ModelLoader.hpp"
#include "SimpleEngineCore/Log.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "SimpleEngineCore/stl_reader.hpp"

namespace SimpleEngine
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 normal;
    };

    static std::vector<Vertex> loadOBJ(const char* file_name)
    {
        //Vertex portions
        std::vector<glm::fvec3> vertex_positions;
        std::vector<glm::fvec2> vertex_texcoords;
        std::vector<glm::fvec3> vertex_normals;

        //Face vectors
        std::vector<GLint> vertex_position_indicies;
        std::vector<GLint> vertex_texcoord_indicies;
        std::vector<GLint> vertex_normal_indicies;

        std::stringstream ss;

        std::ifstream in_file(file_name);
        //File open error check
        if (!in_file.is_open())
        {
            LOG_ERROR("can't open file");
        }

        std::string line;
        //Read one line at a time
        while (std::getline(in_file, line))
        {
            //Get the prefix of the line
            ss.clear();
            ss.str(line);
            std::string prefix;
            ss >> prefix;
            
            if (prefix == "v") //Vertex position
            {
                glm::vec3 temp_vec3;
                ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
                vertex_positions.push_back(temp_vec3);
            }
            else if (prefix == "vt")
            {
                glm::vec2 temp_vec2;
                ss >> temp_vec2.x >> temp_vec2.y;
                //temp_vec2.y = 1.0f - temp_vec2.y;
                vertex_texcoords.push_back(temp_vec2);
            }
            else if (prefix == "vn")
            {
                glm::vec3 temp_vec3;
                ss >> temp_vec3.x >> temp_vec3.y >> temp_vec3.z;
                vertex_normals.push_back(temp_vec3);
            }
            else if (prefix == "f")
            {
                int counter = 0;
                GLint temp_glint = 0;
                while (ss >> temp_glint)
                {
                    //Pushing indices into correct arrays
                    if (counter == 0)
                        vertex_position_indicies.push_back(temp_glint);
                    else if (counter == 1)
                        vertex_texcoord_indicies.push_back(temp_glint);
                    else if (counter == 2)
                        vertex_normal_indicies.push_back(temp_glint);

                    //Handling characters
                    if (ss.peek() == '/')
                    {
                        ++counter;
                        ss.ignore(1, '/');
                    }
                    else if (ss.peek() == ' ')
                    {
                        ++counter;
                        ss.ignore(1, ' ');
                    }

                    //Reset the counter
                    if (counter > 2)
                        counter = 0;
                }
            }
        }

        //Build final vertex array (mesh)
        std::vector<Vertex> vertices;
        vertices.resize(vertex_position_indicies.size(), Vertex());

        //Load in all indices
        for (size_t i = 0; i < vertices.size(); ++i)
        {
            vertices[i].position = vertex_positions[vertex_position_indicies[i] - 1];
            vertices[i].texcoord = vertex_texcoords[vertex_texcoord_indicies[i] - 1];
            vertices[i].normal = vertex_normals[vertex_normal_indicies[i] - 1];
        }
        return vertices;
    }

    Model::Model(const char* path, const char* texture_path)
    {
        std::vector<Vertex> obj = loadOBJ(path);
        vertex_count = obj.size();
        m_p_shader_program = std::make_unique<ShaderProgram>(default_vertex_shader2, default_fragment_shader2);
        model_matrix_uniform_loc = m_p_shader_program->get_uniform_location("model_matrix");
        material.init_shader(*m_p_shader_program);

        m_p_vao = std::make_unique<VertexArray>();
        const BufferLayout buffer_pos_tex_normal
        {
            ShaderDataType::Float3,
            ShaderDataType::Float2,
            ShaderDataType::Float3
        };

        m_p_positions_colors_vbo = std::make_unique<VertexBuffer>(
            obj.data(),
            sizeof(obj[0]) * obj.size(),
            buffer_pos_tex_normal,
            VertexBuffer::EUsage::Static);

        m_p_vao->add_vertex_buffer(*m_p_positions_colors_vbo);

        m_p_texture = std::make_unique<Texture>(texture_path);
        tex0_loc = m_p_shader_program->get_uniform_location("tex0");
        m_p_shader_program->bind();
        glUniform1i(tex0_loc, 0);
    }

	Model::Model(const char* stl_path)
	{
        std::vector<float> coords, normals;
        std::vector<unsigned int> tris, solids;
        try {
            stl_reader::ReadStlFile(stl_path,
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
        m_p_shader_program->bind();
        m_p_vao->bind();
        m_p_vao->enable_vertex_buffer();
        material.update_shader(*m_p_shader_program);
        
        glUniformMatrix4fv(model_matrix_uniform_loc, 1, GL_FALSE,
            glm::value_ptr(model_matrix));

        if (m_p_texture != nullptr)
        {
            m_p_texture->bind();
            glUniform1i(tex0_loc, 0);
        }

        if (m_p_index_buffer == nullptr)
        {
            glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        }
        else
        {
            m_p_vao->set_index_buffer(*m_p_index_buffer);
            glDrawElements(GL_TRIANGLES,
                static_cast<GLsizei>(m_p_vao->get_indices_count()),
                GL_UNSIGNED_INT,
                nullptr);
        }
    }
}
