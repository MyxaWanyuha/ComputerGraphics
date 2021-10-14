#include "Camera.hpp"
#include "SimpleEngineCore/Window.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace SimpleEngine {

Camera::Camera(Window& w, glm::vec3 position)
    : m_window(w),
      m_position(position)
{
}

void Camera::matrix(float FOVdeg, float nearPlane, float farPlane,
            ShaderProgram& shaderProgram, const char* uniform)
{
    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);

    view = glm::lookAt(m_position, m_position + m_orientation, m_up);
    projection = glm::perspective(glm::radians(FOVdeg),
                                  static_cast<float>(m_window.get_width())
                                  / static_cast<float>(m_window.get_height()),
                                  nearPlane,
                                  farPlane);
    glUniformMatrix4fv(shaderProgram.get_uniform_location(uniform), 1, GL_FALSE,
                       glm::value_ptr(projection * view));
}

void Camera::inputs()
{
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_W) == GLFW_PRESS)
    {
        m_position += m_speed * m_orientation;
    }
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_S) == GLFW_PRESS)
    {
        m_position += m_speed * -m_orientation;
    }
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_A) == GLFW_PRESS)
    {
        m_position += m_speed * -glm::normalize(glm::cross(m_orientation, m_up));
    }
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_D) == GLFW_PRESS)
    {
        m_position += m_speed * glm::normalize(glm::cross(m_orientation, m_up));
    }
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        m_position += m_speed * m_up;
    }
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        m_position += m_speed * -m_up;
    }
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        m_speed = 0.4f;
    }
    if(glfwGetKey(m_window.get_window_ptr(), GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        m_speed = 0.1f;
    }

    if(glfwGetMouseButton(m_window.get_window_ptr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        glfwSetInputMode(m_window.get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if(m_firstClick)
        {
            glfwSetCursorPos(m_window.get_window_ptr(), m_window.get_width() / 2, m_window.get_height() / 2);
            m_firstClick = false;
        }

        double mouseX;
        double mouseY;

        glfwGetCursorPos(m_window.get_window_ptr(), &mouseX, &mouseY);

        float rotX = m_sensivity * static_cast<float>(mouseY -
                                                      (m_window.get_height() / 2)) / m_window.get_height();
        float rotY = m_sensivity * static_cast<float>(mouseX -
                                                      (m_window.get_width() / 2)) / m_window.get_width();

        glm::vec3 newOrientation = glm::rotate(m_orientation,
                                               glm::radians(-rotX),
                                               glm::normalize(glm::cross(m_orientation, m_up)));

        if(std::abs(glm::angle(newOrientation, m_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            m_orientation = newOrientation;
        }

        m_orientation = glm::rotate(m_orientation, glm::radians(-rotY), m_up);
        glfwSetCursorPos(m_window.get_window_ptr(), m_window.get_width() / 2, m_window.get_height() / 2);
    }
    else if(glfwGetMouseButton(m_window.get_window_ptr(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        glfwSetInputMode(m_window.get_window_ptr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_firstClick = true;
    }
}

}
