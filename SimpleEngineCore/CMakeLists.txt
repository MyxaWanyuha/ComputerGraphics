cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME SimpleEngineCore)

set(ENGINE_PUBLIC_INCLUDES
    includes/SimpleEngineCore/Application.hpp
    includes/SimpleEngineCore/Types.hpp
    includes/SimpleEngineCore/Log.hpp
    includes/SimpleEngineCore/Event.hpp
)

set(ENGINE_PRIVATE_INCLUDES
    src/SimpleEngineCore/Window.hpp
    src/SimpleEngineCore/stl_reader.hpp
    src/SimpleEngineCore/stb_image.h
    src/SimpleEngineCore/Rendering/OpenGL/ShaderProgram.hpp
    src/SimpleEngineCore/Rendering/OpenGL/VertexBuffer.hpp
    src/SimpleEngineCore/Rendering/OpenGL/VertexArray.hpp
    src/SimpleEngineCore/Rendering/OpenGL/IndexBuffer.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Camera.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Cone.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Cylinder.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Trapezoid.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Torus.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Spiral.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Shape.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Cube.hpp
    src/SimpleEngineCore/Rendering/OpenGL/ModelLoader.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Model.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Texture.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Light.hpp
    src/SimpleEngineCore/Rendering/OpenGL/Material.hpp
	src/SimpleEngineCore/Rendering/OpenGL/ComplexModel.hpp
)

set(ENGINE_PRIVATE_SOURCE
    src/SimpleEngineCore/Application.cpp
    src/SimpleEngineCore/Log.cpp
    src/SimpleEngineCore/Window.cpp
    src/SimpleEngineCore/Rendering/OpenGL/ShaderProgram.cpp
    src/SimpleEngineCore/Rendering/OpenGL/VertexBuffer.cpp
    src/SimpleEngineCore/Rendering/OpenGL/VertexArray.cpp
    src/SimpleEngineCore/Rendering/OpenGL/IndexBuffer.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Camera.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Cone.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Cylinder.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Trapezoid.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Torus.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Spiral.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Shape.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Cube.cpp
    src/SimpleEngineCore/Rendering/OpenGL/ModelLoader.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Model.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Texture.cpp
    src/SimpleEngineCore/Rendering/OpenGL/Light.cpp
	src/SimpleEngineCore/Rendering/OpenGL/ComplexModel.cpp
)

set(ENGINE_ALL_SOURCES
    ${ENGINE_PUBLIC_INCLUDES}
    ${ENGINE_PRIVATE_INCLUDES}
    ${ENGINE_PRIVATE_SOURCE}
)

add_library(${ENGINE_PROJECT_NAME} STATIC
    ${ENGINE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
    ${ENGINE_ALL_SOURCES}
)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PUBLIC glm)

set(IMGUI_INCLUDES
    ../external/imgui/imgui.h
    ../external/imgui/backends/imgui_impl_glfw.h
    ../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
    ../external/imgui/imgui.cpp
    ../external/imgui/backends/imgui_impl_glfw.cpp
    ../external/imgui/backends/imgui_impl_opengl3.cpp
    ../external/imgui/imgui_demo.cpp
    ../external/imgui/imgui_tables.cpp
    ../external/imgui/imgui_widgets.cpp
    ../external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
    ${IMGUI_INCLUDES}
    ${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw glm)

target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)


