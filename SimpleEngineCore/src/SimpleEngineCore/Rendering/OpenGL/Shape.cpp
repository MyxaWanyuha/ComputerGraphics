#include "Shape.hpp"

#include <glm/trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include<glm/common.hpp>

namespace SimpleEngine
{

Shape::Shape()
{
    recalculate_model_matrix();
}

void Shape::set_scale(glm::vec3 new_scale)
{
    if (scale == new_scale)
        return;
    scale = new_scale;
    recalculate_model_matrix();
}

void Shape::set_location(glm::vec3 new_location)
{
    if (location == new_location)
        return;
    location = new_location;
    recalculate_model_matrix();
}

void Shape::set_rotation(glm::vec3 new_rotation)
{
    if (rotation == new_rotation)
        return;
    rotation = new_rotation;
    recalculate_model_matrix();
}

void Shape::recalculate_model_matrix()
{
    model_matrix = make_location_matrix() * make_rotation_matrix() * make_scale_matrix();
}

glm::mat4 Shape::make_scale_matrix() const
{
    return glm::mat4(
        scale.x,       0,       0, 0,
               0,scale.y,       0, 0,
               0,       0,scale.z, 0,
               0,       0,       0, 1);
}

glm::mat4 Shape::make_location_matrix() const
{
    return glm::mat4(1,            0,            0,            0,
                     0,            1,            0,            0,
                     0,            0,            1,            0,
           location.x,  location.y,  location.z,            1);
}

glm::mat4 Shape::make_rotation_matrix() const
{
    return glm::mat4_cast(glm::quat(glm::radians(rotation)));
}

}
