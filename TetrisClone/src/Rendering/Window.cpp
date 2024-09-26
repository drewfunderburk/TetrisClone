#include "Window.h"
#include "Shader.h"

// Define GLEW_STATIC to use the static version of GLEW
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <stdexcept>
#include <iostream>

Window::Window(const char* title, int width, int height)
{
    m_title = title;
	m_width = width;
	m_height = height;
	m_window = nullptr;

	// If initialization fails, throw an exception
	if (!init(title, width, height))
	{
		glfwTerminate();

		m_window = nullptr;
		m_width = 0;
		m_height = 0;
		m_title = nullptr;

		throw std::runtime_error("Failed to initialize window");
	}
}

Window::~Window()
{
	// Destroy window and terminate GLFW
	glfwDestroyWindow(m_window);
	glfwTerminate();

	// Reset window properties so that lingering pointers do not retain residual eroneous data
	m_window = nullptr;
	m_width = 0;
	m_height = 0;
	m_title = nullptr;
}

bool Window::init(const char* title, int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
		return false;

	// Create window
	m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	// If window creation failed, terminate GLFW
	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	// Make the window's context current
	glfwMakeContextCurrent(m_window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	// Create GL buffer for triangle
	float positions[] =
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	// Create GL Shader
	unsigned int shader = Shader::LoadShader("src/Rendering/Basic.shader");
	glUseProgram(shader);

    return true;
}

void Window::update()
{

	if (!glfwWindowShouldClose(m_window))
	{
		// Clear the buffer
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// Swap the buffers
		glfwSwapBuffers(m_window);
		// Poll for events
		glfwPollEvents();
	}
}

void Window::setWindowSize(int width, int height)
{
	m_width = width;
	m_height = height;
	glfwSetWindowSize(m_window, width, height);
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(m_window);
}
