#ifndef WINDOW_HPP
#define WINDOW_HPP
#include "SimpleEngineCore/Types.hpp"
#include "SimpleEngineCore/Event.hpp"
#include <functional>
#include <memory>
struct GLFWwindow;

namespace SimpleEngine {

class Window
{
public:
    using EventCallbackFn = std::function<void(BaseEvent&)>;

    Window(string title, const u32 width, const u32 height);
    ~Window();

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    void on_update();

    u32 get_width() const { return m_data.width; }
    u32 get_height() const { return m_data.height; }
    GLFWwindow* get_window_ptr() const { return m_pWindow; }
    void set_event_callback(const EventCallbackFn& callback)
    {
        m_data.eventCallbackFn = callback;
    }
private:
    struct WindowData
    {
        string title;
        u32 width;
        u32 height;
        EventCallbackFn eventCallbackFn;
    };

    i32 init();
    void shutdown();

    GLFWwindow* m_pWindow = nullptr;
    WindowData m_data;
    float m_background_color[4] = {0.f, 0.f, 0.f, 1.f};

    std::unique_ptr<class ShaderProgram> p_shader_program;
    std::unique_ptr<class VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<class IndexBuffer> p_index_buffer;
    std::unique_ptr<class VertexArray> p_vao;

    std::unique_ptr<class ShaderProgram> p_shader_program2;
    std::unique_ptr<class VertexBuffer> p_positions_colors_vbo2;
    std::unique_ptr<class VertexArray> p_vao2;
    std::unique_ptr<class Cone> p_cone;
    std::unique_ptr<class Cylinder> p_cylinder;
    std::unique_ptr<class Trapezoid> p_trapezoid;
    std::unique_ptr<class Torus> p_torus;

};

}

#endif // WINDOW_HPP
