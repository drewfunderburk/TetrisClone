#include "Window.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

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
    return true;
}

void Window::update()
{

	if (!glfwWindowShouldClose(m_window))
	{
		// Clear the buffer
		glClear(GL_COLOR_BUFFER_BIT);

		// Swap the buffers
		glfwSwapBuffers(m_window);
		// Poll for events
		glfwPollEvents();
	}
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(m_window);
}
