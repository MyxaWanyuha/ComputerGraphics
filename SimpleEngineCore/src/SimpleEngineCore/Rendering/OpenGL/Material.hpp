#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"

namespace SimpleEngine
{

class Material
{
public:
	Material(
		glm::vec3 ambient = glm::vec3(0.1f),
		glm::vec3 diffuse = glm::vec3(1.f),
		glm::vec3 specular = glm::vec3(1.f),
		GLint diffuseTex = 0,
		GLint specularTex = 1)
		: ambient(ambient),
		  diffuse(diffuse),
		  specular(specular),
		  diffuseTex(diffuseTex),
		  specularTex(specularTex)
	{
	}

	void update_shader(const ShaderProgram& program)
	{
		glUniform3fv(uniform_loc_ambient, 1, glm::value_ptr(ambient));
		glUniform3fv(uniform_loc_diffuse, 1, glm::value_ptr(diffuse));
		glUniform3fv(uniform_loc_specular, 1, glm::value_ptr(specular));
		glUniform1i(uniform_loc_diffuseTex, diffuseTex);
		glUniform1i(uniform_loc_specularTex, specularTex);
	}

	void init_shader(const ShaderProgram& program)
	{
		uniform_loc_ambient			= program.get_uniform_location("material.ambient");
		uniform_loc_diffuse			= program.get_uniform_location("material.diffuse");
		uniform_loc_specular		= program.get_uniform_location("material.specular");
		uniform_loc_diffuseTex		= program.get_uniform_location("material.diffuseTex");
		uniform_loc_specularTex		= program.get_uniform_location("material.specularTex");
	}

	glm::vec3	get_ambient() const noexcept { return ambient; }
	glm::vec3	get_diffuse() const noexcept { return diffuse; }
	glm::vec3	get_specular() const noexcept { return specular; }
	GLint		get_diffuseTex() const noexcept { return diffuseTex; }
	GLint		get_specularTex() const noexcept { return specularTex; }

	void set_ambient(glm::vec3	new_ambient)	 { ambient = new_ambient; }
	void set_diffuse(glm::vec3	new_diffuse)	 { diffuse = new_diffuse; }
	void set_specular(glm::vec3	new_specular)	 { specular = new_specular; }
	void set_diffuseTex(GLint	new_diffuseTex)  { diffuseTex = new_diffuseTex; }
	void set_specularTex(GLint	new_specularTex) { specularTex = new_specularTex; }

private:
	glm::vec3	ambient;
	glm::vec3	diffuse;
	glm::vec3	specular;
	GLint		diffuseTex;
	GLint		specularTex;

	i32 uniform_loc_ambient = -1;
	i32 uniform_loc_diffuse = -1;
	i32 uniform_loc_specular = -1;
	i32 uniform_loc_diffuseTex = -1;
	i32 uniform_loc_specularTex = -1;
};

}

#endif // MATERIAL_HPP