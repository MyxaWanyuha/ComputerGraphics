#ifndef LIGHT_HPP
#define LIGHT_HPP
#include <glm/vec3.hpp>
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"

namespace SimpleEngine
{

class Light
{
public:
	Light(float intensity, glm::vec3 color);
	virtual void update_shader(const ShaderProgram& program) = 0;
	virtual ~Light() = default;

protected:
	float intensity;
	glm::vec3 color;
};

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position = glm::vec3(1,1,1), float intensity = 1.f, glm::vec3 color = glm::vec3(1.f, 1.f, 1.f),
		float constant = 1.f, float linear = 0.045f, float quadratic = 0.0075f);
	virtual ~PointLight() override = default;

	virtual void update_shader(const ShaderProgram& program) override;
	void init_shader(const ShaderProgram& program);
	void set_position(const glm::vec3& position);
	glm::vec3 get_position() const { return position; }
protected:
	glm::vec3 position;
	float constant;
	float linear;
	float quadratic;
	i32 uniform_loc_position  = -1;
	i32 uniform_loc_intensity = -1;
	i32 uniform_loc_color     = -1;
	i32 uniform_loc_constant  = -1;
	i32 uniform_loc_linear    = -1;
	i32 uniform_loc_quadratic = -1;
};

}
#endif // LIGHT_HPP