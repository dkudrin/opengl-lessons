#include "VertexBuffer.h"
#include "Renderer.h"

// �������� � ���������� ������� � ������ ����������
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GlCall(glGenBuffers(1, &m_RendererID)); // gives free int ID that we can use for new buffer binding
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // sets buffer ID to given free int ID (1)
	GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // ���������� �������� � OpenGl

}

VertexBuffer::~VertexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // sets buffer ID to given free int ID (1)
}

void VertexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // sets buffer ID to given free int ID (1)
}
