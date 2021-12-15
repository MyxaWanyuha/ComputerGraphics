#ifndef SHAPE_CPP
#define SHAPE_CPP

#include <glm/mat4x4.hpp>

namespace SimpleEngine
{

class Shape
{
public:
    Shape();
    virtual ~Shape() = default;
    virtual void render() = 0;

    glm::vec3 get_scale() const noexcept { return scale; }
    glm::vec3 get_location() const noexcept { return location; }
    glm::vec3 get_rotation() const noexcept { return rotation; }

    void set_scale(glm::vec3 new_scale);
    void set_location(glm::vec3 new_location);
    void set_rotation(glm::vec3 new_rotation);
protected:
    void recalculate_model_matrix();
    glm::mat4 make_scale_matrix() const;
    glm::mat4 make_location_matrix() const;
    glm::mat4 make_rotation_matrix() const;

    glm::mat4 model_matrix;

    const char* default_vertex_shader =
R"(#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
out vec3 color;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
void main() {
    color = vertex_color;
    gl_Position = view_matrix * model_matrix * vec4(vertex_position, 1.0);
})";

    const char* default_fragment_shader =
R"(#version 460
in vec3 color;
out vec4 frag_color;
void main() {
   frag_color = vec4(color, 1.0);
})";


    const char* default_vertex_shader2 =
R"(#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texture;
layout(location = 2) in vec3 vertex_normal;
out vec2 texture_coord;
out vec3 vs_position;
out vec3 vs_normal;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
void main() {
    vs_position = vec4(model_matrix * vec4(vertex_position, 1.f)).xyz;
    vs_normal = mat3(model_matrix) * vertex_normal;
    texture_coord = vertex_texture;
    gl_Position = view_matrix * model_matrix * vec4(vertex_position, 1.0);
})";

    const char* default_fragment_shader2 =
R"(#version 460

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

struct PointLight
{
	vec3 position;
	float intensity;
	vec3 color;
	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform PointLight pointLight;
uniform sampler2D tex0;
uniform vec3 cameraPos;
in vec2 texture_coord;
in vec3 vs_position;
in vec3 vs_normal;
out vec4 frag_color;

vec3 calculateAmbient(Material material)
{
	return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
	vec3 posToLightDirVec = normalize(lightPos0 - vs_position);
	float diffuse = clamp(dot(posToLightDirVec, normalize(vs_normal)), 0, 1);
	vec3 diffuseFinal = material.diffuse * diffuse;

	return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
	vec3 lightToPosDirVec = normalize(vs_position - lightPos0);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 posToViewDirVec = normalize(cameraPos - vs_position);
	float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 35);
	vec3 specularFinal = material.specular * specularConstant * texture(material.specularTex, texture_coord).rgb;
	//vec3 specularFinal = material.specular * specularConstant;

	return specularFinal;
}
void main() {
	float distance = length(pointLight.position - vs_position);
	float attenuation = pointLight.constant / (1.f + pointLight.linear * distance
+ pointLight.quadratic * (distance * distance));

	vec3 ambientFinal = calculateAmbient(material);
	vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, pointLight.position);
	vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, pointLight.position, cameraPos);

	ambientFinal *= attenuation;
	diffuseFinal *= attenuation;
	specularFinal *= attenuation;

	frag_color = 
	texture(material.diffuseTex, texture_coord) * (vec4(ambientFinal, 1.f) 
+ vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));

	//frag_color = (vec4(ambientFinal, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
    //frag_color = texture(tex0, texture_coord);
})";
private:
    glm::vec3 scale{ 1.f, 1.f, 1.f };
    glm::vec3 rotation{ 0.f, 0.f, 0.f };
    glm::vec3 location{ 0.f, 0.f, 0.f };
};

}

#endif // SHAPE_CPP