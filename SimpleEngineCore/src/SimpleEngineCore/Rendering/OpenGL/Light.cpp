#include "Light.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace SimpleEngine
{
	Light::Light(float intensity, glm::vec3 color)
		: intensity(intensity),
		  color(color)
	{
	}

	PointLight::PointLight(glm::vec3 position, float intensity, glm::vec3 color,
		float constant, float linear, float quadratic)
		: position(position),
		  Light(intensity, color),
		  constant(constant),
		  linear(linear),
		  quadratic(quadratic)
	{
	}

	void PointLight::update_shader(const ShaderProgram& program) const
	{
		program.bind();
		glUniform3fv(uniform_loc_position, 1, glm::value_ptr(position));
		glUniform1f(uniform_loc_intensity, intensity);
		glUniform3fv(uniform_loc_color, 1, glm::value_ptr(color));
		glUniform1f(uniform_loc_constant, constant);
		glUniform1f(uniform_loc_linear, linear);
		glUniform1f(uniform_loc_quadratic, quadratic);
	}

	void PointLight::init_shader(const ShaderProgram& program)
	{
		program.bind();
		uniform_loc_position	= program.get_uniform_location("pointLight.position");
		uniform_loc_intensity	= program.get_uniform_location("pointLight.intensity");
		uniform_loc_color		= program.get_uniform_location("pointLight.color");
		uniform_loc_constant	= program.get_uniform_location("pointLight.constant");
		uniform_loc_linear		= program.get_uniform_location("pointLight.linear");
		uniform_loc_quadratic	= program.get_uniform_location("pointLight.quadratic");
	}

	void PointLight::set_position(const glm::vec3& position)
	{
		this->position = position;
	}
}