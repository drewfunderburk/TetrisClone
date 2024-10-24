#include "Window.h"
#include "Shader.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <stdexcept>
#include <iostream>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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

	m_vertexBuffer->unbind();
	m_indexBuffer->unbind();
	m_vertexArray->unbind();
	m_shader->Unbind();
	delete m_vertexBuffer;
	delete m_indexBuffer;
	delete m_vertexArray;
	delete m_shader;
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_vertexArray = nullptr;
	m_shader = nullptr;
}

bool Window::init(const char* title, int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
		return false;

	// Set core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Print GLFW Version
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	std::cout << "GLFW Version: " << major << "." << minor << "." << revision << std::endl;

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

	// Set GLFW debug message callback
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	glfwSetErrorCallback(GLFWErrorMessageCallback);

	// Print GLEW Version
	std::cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << std::endl;

	// Print OpenGL Version
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

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

	// Create vertex array object
	m_vertexArray = new VertexArray();
	m_vertexBuffer = new VertexBuffer(positions, 4 * 2 * sizeof(float));
	VertexBufferLayout layout;
	layout.push<float>(2);
	m_vertexArray->addBuffer(*m_vertexBuffer, layout);

	// Enable vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// Create index buffer object
	m_indexBuffer = new IndexBuffer(indices, 6);

	// Create shader
	m_shader = new Shader("res/shaders/Basic.shader");
	m_shader->Bind();

	// Unbind the buffer
	m_vertexArray->unbind();
	m_shader->Unbind();
	m_vertexBuffer->unbind();
	m_indexBuffer->unbind();

    return true;
}

void Window::update()
{

	if (!glfwWindowShouldClose(m_window))
	{
		// Clear the buffer
		glClear(GL_COLOR_BUFFER_BIT);

		m_shader->Bind();
		m_vertexArray->bind();
		m_indexBuffer->bind();

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

void Window::GLFWErrorMessageCallback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << std::endl;
}