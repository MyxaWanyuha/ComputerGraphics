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

namespace SimpleEngine {

static bool s_GLFW_initialized = false;

/*GLfloat positions_colors_triangle[] = {
     0.0f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 1.0f
};

GLfloat positions_colors_square[] = {
    -0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,  0.4f, 1.0f, 0.3f
};

GLuint indices_square[] = {
    0, 1, 2, 3, 2, 1
};*/
#define ColorRed     1.0f, 0.0f, 0.0f,
#define ColorGreen   0.0f, 1.0f, 0.0f,
#define ColorBlue    0.0f, 0.0f, 1.0f,
#define ColorCyan    0.0f, 1.0f, 1.0f,
#define ColorOrange  1.0f, 0.0f, 1.0f,
#define ColorYellow  1.0f, 1.0f, 0.0f,
#define ColorBlue5   0.5f, 0.7f, 0.9f,
#define ColorWhite   1.0f, 1.0f, 1.0f,
#define ColorBlack   0.0f, 0.0f, 0.0f,

GLfloat cube_positions_colors[] = {
  -0.5f, -0.5f,  0.5f,    ColorRed   //front
   0.5f, -0.5f,  0.5f,    ColorGreen
   0.5f,  0.5f,  0.5f,    ColorBlue
  -0.5f,  0.5f,  0.5f,    ColorWhite
  -0.5f, -0.5f, -0.5f,    ColorCyan  //back
   0.5f, -0.5f, -0.5f,    ColorOrange
   0.5f,  0.5f, -0.5f,    ColorYellow
  -0.5f,  0.5f, -0.5f,    ColorBlue5
};
// тор, спираль
GLfloat cube_positions_colors2[] = {
  -1.0f, -1.0f,  1.0f,    ColorRed   //front
   1.0f, -1.0f,  1.0f,    ColorBlue
   1.0f,  1.0f,  1.0f,    ColorOrange
  -1.0f,  1.0f,  1.0f,    ColorYellow
  -1.0f, -1.0f, -1.0f,    ColorRed  //back
   1.0f, -1.0f, -1.0f,    ColorGreen
   1.0f,  1.0f, -1.0f,    ColorBlue5
  -1.0f,  1.0f, -1.0f,    ColorWhite
};

GLuint indices_cube[] = {
    0,1,1,2,2,3,3,0,//front
    4,5,5,6,6,7,7,4,//back
    0,4,1,5,2,6,3,7,
};

i32 DrawType = GL_LINES;

std::unique_ptr<Camera> p_camera;

const char* vertex_shader = R"(
#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
out vec3 color;
uniform mat4 camMatrix;
uniform mat4 transform;
void main() {
    color = vertex_color;
    gl_Position = camMatrix * transform * vec4(vertex_position, 1.0);
})";

const char* fragment_shader = R"(
#version 460
in vec3 color;
out vec4 frag_color;
void main() {
   frag_color = vec4(color, 1.0);
})";
glm::mat4 unitary(1);

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

    // OpenGL

    p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
    if(!p_shader_program->isCompiled())
    {
        return false;
    }
    p_shader_program2 = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
    if (!p_shader_program2->isCompiled())
    {
        return false;
    }
    
    const BufferLayout buffer_layout_2_vec3
    {
        ShaderDataType::Float3,
        ShaderDataType::Float3
    };

    p_camera = std::make_unique<Camera>(*this, glm::vec3(0.0f, 0.0f, 2.0f));
    {
        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>(cube_positions_colors, sizeof(cube_positions_colors), buffer_layout_2_vec3);
        p_index_buffer = std::make_unique<IndexBuffer>(indices_cube, sizeof(indices_cube) / sizeof(indices_cube[0]));
        p_vao->add_vertex_buffer(*p_positions_colors_vbo);
        p_vao->set_index_buffer(*p_index_buffer);
    }


    p_positions_colors_vbo2 = std::make_unique<VertexBuffer>(cube_positions_colors2, sizeof(cube_positions_colors2), buffer_layout_2_vec3);
    p_vao2 = std::make_unique<VertexArray>();
    p_vao2->add_vertex_buffer(*p_positions_colors_vbo2);
    p_vao2->set_index_buffer(*p_index_buffer);

    p_cylinder = std::make_unique<Cylinder>(1.f, 0.5f, 0.5f, 0.5f, 0, 10, glm::vec3{ 1, 1, 1 });
    p_trapezoid = std::make_unique<Trapezoid>(3, 0.5f, 1, glm::vec3{1, 0.6, 1}, glm::vec3{1, 0, 1});
    p_cone = std::make_unique<Cone>(1.f, 0.5f, -0.5f, -0.5f, 0, 10, glm::vec3{ 0.1, 0.5, 0.7 });
    p_torus = std::make_unique<Torus>(1.5f, 100, 0.5f, 100, glm::vec3{ 0.2f, 1, 0.5f }, glm::vec3{ 1,0.5f, 0.2f });
    // OpenGL end

    return 0;
}

float angle = 0.0f;
void Window::on_update()
{
    glClearColor(m_background_color[0], m_background_color[1], m_background_color[2], m_background_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(get_width());
    io.DisplaySize.y = static_cast<float>(get_height());

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::Begin("Background Color Window");
    ImGui::ColorEdit4("Background Color", m_background_color);

    p_camera->inputs();

    angle += 0.01f;

    p_camera->matrix(45.0f, 0.1f, 100.0f, *p_shader_program2, "camMatrix");
    p_shader_program2->bind();
    glUniformMatrix4fv(p_shader_program2->get_uniform_location("transform"), 1, GL_FALSE, glm::value_ptr(glm::rotate(angle, glm::vec3{ 0,1,0 })));
    p_vao2->enable_vertex_buffer();
    p_vao2->set_index_buffer(*p_index_buffer);
    p_vao2->bind();
    glDrawElements(DrawType, static_cast<GLsizei>(p_vao2->get_indices_count()), GL_UNSIGNED_INT, nullptr);


    p_camera->matrix(45.0f, 0.1f, 100.0f, p_cone->getShaderProgram(), "camMatrix");
    p_cone->rotate(glm::vec3{ 1,1,0 }, 0.01f);

    p_camera->matrix(45.0f, 0.1f, 100.0f, p_cylinder->getShaderProgram(), "camMatrix");
    p_cylinder->rotate_render(glm::vec3{ 1,0,0 }, 0.01f);
    
    p_camera->matrix(45.0f, 0.1f, 100.0f, p_trapezoid->getShaderProgram(), "camMatrix");
    p_trapezoid->rotate(glm::vec3{ 0,0,1 }, 0.01f);

    p_camera->matrix(45.0f, 0.1f, 100.0f, p_torus->getShaderProgram(), "camMatrix");
    p_torus->rotate(glm::vec3{ 0,0,1 }, 0.01f);
    
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
