#pragma once

class IndexBuffer
{
public:
	IndexBuffer() : m_rendererID(0), m_count(0) {}
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return m_count; }

private:
	unsigned int m_rendererID;
	unsigned int m_count;
};