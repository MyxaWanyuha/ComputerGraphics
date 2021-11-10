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

    const char* default_light_vert =
R"(#version 460

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
// Normals (not necessarily normalized)
layout (location = 3) in vec3 aNormal;


// Outputs the color for the Fragment Shader
out vec3 color;
// Outputs the texture coordinates to the Fragment Shader
out vec2 texCoord;
// Outputs the normal for the Fragment Shader
out vec3 Normal;
// Outputs the current position for the Fragment Shader
out vec3 crntPos;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;
// Imports the model matrix from the main function
uniform mat4 model;


void main()
{
	// calculates current position
	crntPos = vec3(model * vec4(aPos, 1.0f));
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(crntPos, 1.0);

	// Assigns the colors from the Vertex Data to "color"
	color = aColor;
	// Assigns the texture coordinates from the Vertex Data to "texCoord"
	texCoord = aTex;
	// Assigns the normal from the Vertex Data to "Normal"
	Normal = aNormal;
})";

    const char* default_light_frag = 
R"(#version 460

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the color from the Vertex Shader
in vec3 color;
// Imports the texture coordinates from the Vertex Shader
in vec2 texCoord;
// Imports the normal from the Vertex Shader
in vec3 Normal;
// Imports the current position from the Vertex Shader
in vec3 crntPos;

// Gets the Texture Unit from the main function
uniform sampler2D tex0;
// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

void main()
{
	// ambient lighting
	float ambient = 0.20f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// outputs final color
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
})";

private:
    glm::vec3 scale{ 1.f, 1.f, 1.f };
    glm::vec3 rotation{ 0.f, 0.f, 0.f };
    glm::vec3 location{ 0.f, 0.f, 0.f };
};

}

#endif // SHAPE_CPP