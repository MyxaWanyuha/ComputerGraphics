#include "VertexBuffer.hpp"
#include "SimpleEngineCore/Log.hpp"
#include <glad/glad.h>

namespace SimpleEngine {

constexpr u32 shader_data_type_to_components_count(const ShaderDataType type)
{
    switch (type)
    {
        case SimpleEngine::ShaderDataType::Float:
        case SimpleEngine::ShaderDataType::Int:
            return 1;

        case SimpleEngine::ShaderDataType::Float2:
        case SimpleEngine::ShaderDataType::Int2:
            return 2;

        case SimpleEngine::ShaderDataType::Float3:
        case SimpleEngine::ShaderDataType::Int3:
            return 3;

        case SimpleEngine::ShaderDataType::Float4:
        case SimpleEngine::ShaderDataType::Int4:
            return 4;
    }

    LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
    return 0;
}

constexpr size_t shader_data_type_size(const ShaderDataType type)
{
    switch(type)
    {
    case SimpleEngine::ShaderDataType::Float:
    case SimpleEngine::ShaderDataType::Float2:
    case SimpleEngine::ShaderDataType::Float3:
    case SimpleEngine::ShaderDataType::Float4:
        return sizeof(GLfloat) * shader_data_type_to_components_count(type);

    case SimpleEngine::ShaderDataType::Int:
    case SimpleEngine::ShaderDataType::Int2:
    case SimpleEngine::ShaderDataType::Int3:
    case SimpleEngine::ShaderDataType::Int4:
        return sizeof(GLint) * shader_data_type_to_components_count(type);
    }

    LOG_ERROR("shader_data_type_size: unknown ShaderDataType!");
    return 0;
}

constexpr u32 shader_data_type_to_component_type(const ShaderDataType type)
{
    switch(type)
    {
    case SimpleEngine::ShaderDataType::Float:
    case SimpleEngine::ShaderDataType::Float2:
    case SimpleEngine::ShaderDataType::Float3:
    case SimpleEngine::ShaderDataType::Float4:
        return GL_FLOAT;

    case SimpleEngine::ShaderDataType::Int:
    case SimpleEngine::ShaderDataType::Int2:
    case SimpleEngine::ShaderDataType::Int3:
    case SimpleEngine::ShaderDataType::Int4:
        return GL_INT;
    }

    LOG_ERROR("shader_data_type_to_component_type: unknown ShaderDataType!");
    return GL_FLOAT;
}

constexpr GLenum usage_to_GLenum(const VertexBuffer::EUsage usage)
{
    switch(usage)
    {
    case SimpleEngine::VertexBuffer::EUsage::Static:
        return GL_STATIC_DRAW;
    case SimpleEngine::VertexBuffer::EUsage::Dynamic:
        return GL_DYNAMIC_DRAW;
    case SimpleEngine::VertexBuffer::EUsage::Stream:
        return GL_STREAM_DRAW;
    }
    LOG_ERROR("Unknown VertexBuffer usage");
    return GL_STREAM_DRAW;
}

BufferElement::BufferElement(const ShaderDataType type)
    : type(type),
      component_type(shader_data_type_to_component_type(type)),
      components_count(shader_data_type_to_components_count(type)),
      size(shader_data_type_size(type)),
      offset(0)
{
}

VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout buffer_layout, const EUsage usage)
    : m_buffer_layout(std::move(buffer_layout))
{
    glGenBuffers(1, &m_id);
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, data, usage_to_GLenum(usage));
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept
{
    m_id = vertexBuffer.m_id;
    vertexBuffer.m_id = 0;
    return *this;
}

VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
    : m_id(vertexBuffer.m_id),
      m_buffer_layout(std::move(vertexBuffer.m_buffer_layout))
{
    vertexBuffer.m_id = 0;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

}
