#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "SimpleEngineCore/Types.hpp"
#include "ShaderProgram.hpp"

namespace SimpleEngine {

class Camera
{
public:
    Camera(class Window& w, glm::vec3 position);
    void matrix(float FOVdeg, float nearPlane, float farPlane,
                ShaderProgram& shaderProgram, const char* uniform);
    void inputs();
private:
    const Window& m_window;
    glm::vec3 m_position;
    glm::vec3 m_orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    float m_speed = 0.1f;
    float m_sensivity = 100.0f;
    bool m_firstClick = true;
};

}

#endif // CAMERA_HPP
