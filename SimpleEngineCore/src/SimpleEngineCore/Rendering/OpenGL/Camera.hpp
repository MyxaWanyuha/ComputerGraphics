#ifndef CAMERA_HPP
#define CAMERA_HPP
#include "SimpleEngineCore/Types.hpp"
#include "ShaderProgram.hpp"

namespace SimpleEngine {

class Camera
{
public:
    Camera(class Window& w, glm::vec3 position);
    void update_matrix(float FOVdeg, float nearPlane, float farPlane);
    void set_matrix(const ShaderProgram& shaderProgram, const char* uniform) const;
    void set_position(const ShaderProgram& shaderProgram, const char* uniform) const;
    void inputs();
    void SetSpeed(float speed) { m_speed = speed; }
    void MoveForward();
    void MovaBackward();
    void MoveLeft();
    void MovaRight();
    void MoveUp();
    void MoveDown();
private:
    const Window& m_window;
    glm::vec3 m_position;
    glm::vec3 m_orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 camera_matrix = glm::mat4(1.0f);
    float m_speed = 0.1f;
    float m_sensivity = 100.0f;
    bool m_firstClick = true;
};

}

#endif // CAMERA_HPP
