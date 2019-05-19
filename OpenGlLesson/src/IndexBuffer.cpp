#include "IndexBuffer.h"
#include "Renderer.h"

// Создание и наполнение буффера индексов в памяти видеокарты
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	GlCall(glGenBuffers(1, &m_RendererID)); // gives free int ID that we can use for new buffer binding
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // sets buffer ID to given free int ID (2)
	GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW)); // Отправляет вертексы в OpenGl

}

IndexBuffer::~IndexBuffer()
{
	GlCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); // sets buffer ID to given free int ID (1)
}

void IndexBuffer::Unbind() const
{
	GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // sets buffer ID to given free int ID (1)
}
