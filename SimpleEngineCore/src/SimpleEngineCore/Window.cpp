#include "SimpleEngineCore/Window.hpp"
#include "SimpleEngineCore/Log.hpp"

#include "SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp"

#include "SimpleEngineCore/Rendering/OpenGL/Camera.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Cone.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Cylinder.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Trapezoid.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Torus.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Spiral.hpp"

#include "SimpleEngineCore/Rendering/OpenGL/Cube.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Model.hpp"
#include "SimpleEngineCore/Rendering/OpenGL/Light.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/transform.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm/gtc/type_ptr.hpp>

#include<filesystem>
namespace fs = std::filesystem;

namespace SimpleEngine {

static bool s_GLFW_initialized = false;

Window::Window(string title, const u32 width, const u32 height)
    : m_data({std::move(title), width, height})
{
    //i32 resultCode =
            init();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
}

Window::~Window()
{
    shutdown();
}

i32 Window::init()
{
    LOG_INFO("Creating window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);

    if(s_GLFW_initialized == false)
    {
        if (!glfwInit())
        {
            LOG_CRITICAL("Can't initialize GLFW!");
            return -1;
        }
        s_GLFW_initialized = true;
    }

    m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
    if (!m_pWindow)
    {
        LOG_CRITICAL("Can't create window {0} with size {1}x{2}", m_data.title, m_data.width, m_data.height);
        glfwTerminate();
        return -2;
    }

    glfwMakeContextCurrent(m_pWindow);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG_CRITICAL("Failed to initialize GLAD");
        return -3;
    }
    
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glDepthRange(0.0f, 1.0f);

    glfwSetWindowUserPointer(m_pWindow, &m_data);
    glfwSetWindowSizeCallback(m_pWindow,
        [](GLFWwindow* pWindow, int width, int height)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            data.width = width;
            data.height = height;

            EventWindowResize event(width, height);
            data.eventCallbackFn(event);
        });

    glfwSetCursorPosCallback(m_pWindow,
        [](GLFWwindow* pWindow, double x, double y)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            EventMouseMoved event(x, y);
            data.eventCallbackFn(event);
        });

    glfwSetWindowCloseCallback(m_pWindow,
        [](GLFWwindow* pWindow)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));
            EventWindowClose event;
            data.eventCallbackFn(event);
        });

    glfwSetFramebufferSizeCallback(m_pWindow,
        [](GLFWwindow* pWindow, int width, int height)
        {
            glViewport(0, 0, width, height);
        });

    glfwSetKeyCallback(m_pWindow, 
        [](GLFWwindow* pWindow, i32 key, i32 scancode, i32 action, i32 mods)
        {
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(pWindow));

            if (action == GLFW_PRESS)
            {
                EventKeyPressed event;
                event.key = key;
                event.scancode = scancode;
                event.action = action;
                event.mods = mods;
                data.eventCallbackFn(event);
            }
            else if (action == GLFW_RELEASE)
            {
                EventKeyReleased event;
                event.key = key;
                event.scancode = scancode;
                event.action = action;
                event.mods = mods;
                data.eventCallbackFn(event);
            }
            else//GLFW_REPEAT
            {
                
            }
        });

    p_camera = std::make_unique<Camera>(*this, glm::vec3(0.0f, 0.0f, 2.0f));

    //p_cylinder = std::make_unique<Cylinder>(1.f, 0.5f, 0.5f, 0.5f, 0, 10, glm::vec3{ 1, 1, 1 });
    //p_trapezoid = std::make_unique<Trapezoid>(3, 0.5f, 1, glm::vec3{1, 0.6, 1}, glm::vec3{1, 0, 1});
    //p_cone = std::make_unique<Cone>(1.f, 0.5f, -0.5f, -0.5f, 0, 10, glm::vec3{ 0.1, 0.5, 0.7 });
    //p_torus = std::make_unique<Torus>(1.0f, 20, 0.5f, 20, glm::vec3{ 0.2f, 1, 0.5f }, glm::vec3{ 3,2.0f, 1.2f });
    //p_spiral = std::make_unique<Spiral>(glm::veLight.cppc3{ 1,1,1 }, glm::vec3{ 1,2,3 });

    p_shape_cube = std::make_unique<Cube>();
    p_shape_cube->set_location({ 0, 0, 2.1f });
    p_shape_triangle_cube = std::make_unique<TriangleCube>();

    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
    LOG_INFO("parentDir: " + parentDir);
    p_model_rei_obj = std::make_unique<Model>(
        (parentDir + "\\resources\\rei_textured\\rei_textured.obj").c_str(),
        (parentDir + "\\resources\\rei_textured\\rei_texture2.png").c_str());//
    p_model_rei_obj->set_rotation({ 90, -45, 0 });
    p_model_rei_obj->set_location({ 0, 1.1f, 1.0f });
    const float model_scale = 0.01f;
    p_model_rei_obj->set_scale({ model_scale, model_scale, model_scale });

    p_point_light = std::make_unique<PointLight>(glm::vec3(-1, 4, 3));
    p_point_light->init_shader(p_model_rei_obj->get_shader_program());
    
    p_shape_model_rei_stl = std::make_unique<Model>(
        (parentDir + "\\resources\\rei-full-character-and-base.stl").c_str());
    p_shape_model_rei_stl->set_rotation({0, -45, 0});
    p_shape_model_rei_stl->set_location({ 0, 1.1f, -1.0f });
    p_shape_model_rei_stl->set_scale({ model_scale, model_scale, model_scale });


    return 0;
}

void Window::on_update()
{
    glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(get_width());
    io.DisplaySize.y = static_cast<float>(get_height());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Editor");
    ImGui::ColorEdit4("Background Color", m_background_color);

    p_camera->inputs();
    p_camera->update_matrix(45.0f, 0.1f, 100.0f);
    //static float angle = 0.0f;
    //angle += 0.01f;
    
    //p_camera->matrix(45.0f, 0.1f, 100.0f, p_cone->getShaderProgram(), "camMatrix");
    //p_cone->rotate(glm::vec3{ 1,1,0 }, 0.01f);

    //p_camera->matrix(45.0f, 0.1f, 100.0f, p_cylinder->getShaderProgram(), "camMatrix");
    //p_cylinder->rotate_render(glm::vec3{ 1,0,0 }, 0.01f);

    //p_camera->matrix(45.0f, 0.1f, 100.0f, p_trapezoid->getShaderProgram(), "camMatrix");
    //p_trapezoid->rotate(glm::vec3{ 0,0,1 }, 0.01f);

    //p_camera->matrix(45.0f, 0.1f, 100.0f, p_torus->getShaderProgram(), "camMatrix");
    //p_torus->rotate(glm::vec3{ 0,0,1 }, 0.01f);
    
    //p_camera->matrix(45.0f, 0.1f, 100.0f, p_spiral->getShaderProgram(), "camMatrix");
    //p_spiral->rotate(glm::vec3{ 0,0,1 }, 0.0f);


    static glm::vec3 scale = p_model_rei_obj->get_scale();
    static glm::vec3 rotation = p_model_rei_obj->get_rotation();
    static glm::vec3 location = p_model_rei_obj->get_location();
    static glm::vec3 light_position = p_point_light->get_position();
    ImGui::InputFloat3("Scale", glm::value_ptr(scale));
    ImGui::InputFloat3("Rotation", glm::value_ptr(rotation));
    ImGui::InputFloat3("Location", glm::value_ptr(location));
    ImGui::InputFloat3("Light Position", glm::value_ptr(light_position));
    p_point_light->set_position(light_position);

    p_model_rei_obj->set_scale(scale);
    p_model_rei_obj->set_location(location);
    p_model_rei_obj->set_rotation(rotation);
    p_camera->set_matrix(p_model_rei_obj->get_shader_program(), "view_matrix");
    p_camera->set_position(p_model_rei_obj->get_shader_program(), "cameraPos");
    p_point_light->update_shader(p_model_rei_obj->get_shader_program());
    p_model_rei_obj->render();
    
    p_camera->set_matrix(p_shape_model_rei_stl->get_shader_program(), "view_matrix");
    p_point_light->update_shader(p_shape_model_rei_stl->get_shader_program());
    p_shape_model_rei_stl->render();
    

    p_camera->set_matrix(p_shape_triangle_cube->get_shader_program(), "view_matrix");
    p_point_light->update_shader(p_shape_triangle_cube->get_shader_program());
    p_shape_triangle_cube->render();

    p_camera->set_matrix(p_shape_cube->get_shader_program(), "view_matrix");
    p_point_light->update_shader(p_shape_cube->get_shader_program());
    p_shape_cube->render();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_pWindow);
    glfwPollEvents();
}

void Window::shutdown()
{
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

}
