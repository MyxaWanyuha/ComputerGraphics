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
out vec2 texture_coord;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
void main() {
    gl_Position = view_matrix * model_matrix * vec4(vertex_position, 1.0);
    texture_coord = vertex_texture;
})";

    const char* default_fragment_shader2 =
R"(#version 460
out vec4 frag_color;

in vec2 texture_coord;
uniform sampler2D tex0;

void main() {
   frag_color = texture(tex0, texture_coord);
})";
private:
    glm::vec3 scale{ 1.f, 1.f, 1.f };
    glm::vec3 rotation{ 0.f, 0.f, 0.f };
    glm::vec3 location{ 0.f, 0.f, 0.f };
};

}

#endif // SHAPE_CPP