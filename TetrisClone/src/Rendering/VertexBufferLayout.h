#pragma once
#include <vector>
#include <stdexcept>
#include <GL/glew.h>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return 4;
		case GL_UNSIGNED_INT: return 4;
		case GL_UNSIGNED_BYTE: return 1;
		}

		//std::runtime_error(false);
		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout() : m_stride(0) {}

	template<typename T>
	void push(unsigned int count)
	{
		std::runtime_error(false, "Unsupported type");
	}

	template<>
	void push<float>(unsigned int count)
	{
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void push<unsigned int>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void push<unsigned char>(unsigned int count)
	{
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline unsigned int getStride() const { return m_stride; }

	inline const std::vector<VertexBufferElement>& getElements() const { return m_elements; }

private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
};