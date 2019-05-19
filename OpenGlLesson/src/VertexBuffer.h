#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID; // Возможно это станет ID для более общего использования, не только для OpenGL 
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};