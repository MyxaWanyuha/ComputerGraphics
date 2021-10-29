#ifndef SPIRAL_HPP
#define SPIRAL_HPP

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

class Spiral
{
public:
	Spiral(const glm::vec3& color = { 1,1,1 }, const glm::vec3& center = { 0,0,0 });
	void rotate(glm::vec3 axis = { 1,0,0 }, float angle = 0.01f);

	const VertexArray& getVertexArray() const { return *m_p_vao; }
	const ShaderProgram& getShaderProgram() const { return *m_p_shader_program; }
private:
	void generate(const glm::vec3& color, const glm::vec3& center);
	std::vector<GLfloat> positionsColors;
	std::vector<GLuint> indices;

	float rotAngle = 0;
	i32 rotationUniformLoc;
	std::unique_ptr<ShaderProgram> m_p_shader_program;
	std::unique_ptr<VertexArray> m_p_vao;
	std::unique_ptr<VertexBuffer> m_p_positions_colors_vbo;
	std::unique_ptr<IndexBuffer> m_p_index_buffer;
};

}

#endif // SPIRAL_HPP