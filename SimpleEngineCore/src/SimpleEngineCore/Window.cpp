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
#include <SimpleEngineCore/Rendering/OpenGL/ComplexModel.hpp>
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

    std::string parentDir = (fs::current_path().fs::path::parent_path()).string();
    LOG_INFO("parentDir: " + parentDir);

    zelda = std::make_unique<ComplexModel>(
        ComplexModel({
            ModelData{ (parentDir + "\\resources\\zelda\\eyes.obj").c_str(), (parentDir + "\\resources\\zelda\\textures\\eyes_diff.png").c_str() },
            ModelData{ (parentDir + "\\resources\\zelda\\hair.obj").c_str(), (parentDir + "\\resources\\zelda\\textures\\hair_diff.png").c_str() },
            ModelData{ (parentDir + "\\resources\\zelda\\mouth.obj").c_str(), (parentDir + "\\resources\\zelda\\textures\\zelda_diff.png").c_str() },
            ModelData{ (parentDir + "\\resources\\zelda\\sheikaSlate.obj").c_str(), (parentDir + "\\resources\\zelda\\textures\\misc_diff.png").c_str()},
            ModelData{ (parentDir + "\\resources\\zelda\\terrain.obj").c_str(), (parentDir + "\\resources\\zelda\\textures\\setCave_diff.png").c_str()},
            ModelData{ (parentDir + "\\resources\\zelda\\torch.obj").c_str(), (parentDir + "\\resources\\zelda\\textures\\misc_diff.png").c_str()},
            ModelData{ (parentDir + "\\resources\\zelda\\fire.obj").c_str(), (parentDir + "\\resources\\zelda\\textures\\fire_diff.png").c_str() },
        }));

    zelda->set_material(Material(glm::vec3(1.f)), 6);
    zelda->set_material(Material(glm::vec3(0.1f), glm::vec3(1.f), glm::vec3(1.f), 0, 0), 2);
    zelda->set_location({ 0, -1, -1 });

    p_point_light = std::make_unique<PointLight>(glm::vec3(-1, 4, 3));
    p_point_light->init_shader(zelda->get_shader_program());
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

    static glm::vec3 scale = zelda->get_scale();
    static glm::vec3 rotation = zelda->get_rotation();
    static glm::vec3 location = zelda->get_location();
    static glm::vec3 light_position = p_point_light->get_position();
    ImGui::InputFloat3("Scale", glm::value_ptr(scale));
    ImGui::InputFloat3("Rotation", glm::value_ptr(rotation));
    ImGui::InputFloat3("Location", glm::value_ptr(location));
    ImGui::InputFloat3("Light Position", glm::value_ptr(light_position));
    ImGui::End();
    p_point_light->set_position(light_position);
    zelda->set_scale(scale);
    zelda->set_location(location);
    zelda->set_rotation(rotation);

    int i = 0;
    auto make_material_edit_widget = [&](const std::string& name, int number)
    {
        ImGui::Begin(name.c_str());
        const auto& material = zelda->get_material(number);
        glm::vec3 l_ambient = material.get_ambient();
        glm::vec3 l_diffuse = material.get_diffuse();
        glm::vec3 l_specular = material.get_specular();
        GLint	  l_diffuseTex = material.get_diffuseTex();
        GLint	  l_specularTex = material.get_specularTex();
        ImGui::InputFloat3("Ambient", glm::value_ptr(l_ambient));
        ImGui::InputFloat3("Diffuse", glm::value_ptr(l_diffuse));
        ImGui::InputFloat3("Specular", glm::value_ptr(l_specular));
        ImGui::InputInt("DiffuseTex", &l_diffuseTex);
        ImGui::InputInt("SpecularTex", &l_specularTex);
        zelda->set_material(Material(l_ambient, l_diffuse, l_specular, l_diffuseTex, l_specularTex), number);

        ImGui::End();
    };
    make_material_edit_widget("eyes", i++);
    make_material_edit_widget("hair", i++);
    make_material_edit_widget("mouth", i++);
    make_material_edit_widget("sheikaSlate", i++);
    make_material_edit_widget("terrain", i++);
    make_material_edit_widget("torch", i++);
    make_material_edit_widget("fire", i++);

    zelda->update_camera(*p_camera, "view_matrix", "cameraPos");
    zelda->update_light(*p_point_light);
    zelda->Render();

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
