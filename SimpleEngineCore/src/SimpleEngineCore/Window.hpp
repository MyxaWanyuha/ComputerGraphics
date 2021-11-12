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
    float m_background_color[4] = {0.271f, 0.232f, 0.451f, 1.f};

    std::unique_ptr<class Camera> p_camera;

    std::unique_ptr<class Cone> p_cone;
    std::unique_ptr<class Cylinder> p_cylinder;
    std::unique_ptr<class Trapezoid> p_trapezoid;
    std::unique_ptr<class Torus> p_torus;
    std::unique_ptr<class Spiral> p_spiral;

    std::unique_ptr<class Cube> p_shape_cube;
    std::unique_ptr<class TriangleCube> p_shape_triangle_cube;

    std::unique_ptr<class Model> p_model_rei_obj;
    std::unique_ptr<class Model> p_shape_model_rei_stl;
};

}

#endif // WINDOW_HPP
