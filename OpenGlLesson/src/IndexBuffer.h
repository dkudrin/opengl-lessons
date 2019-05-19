#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID; // ¬озможно это станет ID дл€ более общего использовани€, не только дл€ OpenGL
	unsigned int m_Count; // —колько индексов у нас есть
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind()  const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};