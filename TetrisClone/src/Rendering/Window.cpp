#include "Window.h"
#include "Shader.h"

// Define GLEW_STATIC to use the static version of GLEW
#define GLEW_STATIC
#define GLEW_DEBUG
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
	glDebugMessageCallback(GLFWDebugMessageCallback, 0);

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

void Window::GLFWDebugMessageCallback(unsigned int source, unsigned int type, unsigned int id, unsigned int severity, int length, const char* message, const void* userParam)
{
	const char* _source;
	const char* _type;
	const char* _severity;

	switch (source) {
	case GL_DEBUG_SOURCE_API:
		_source = "API";
		break;

	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		_source = "WINDOW SYSTEM";
		break;

	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		_source = "SHADER COMPILER";
		break;

	case GL_DEBUG_SOURCE_THIRD_PARTY:
		_source = "THIRD PARTY";
		break;

	case GL_DEBUG_SOURCE_APPLICATION:
		_source = "APPLICATION";
		break;

	case GL_DEBUG_SOURCE_OTHER:
		_source = "UNKNOWN";
		break;

	default:
		_source = "UNKNOWN";
		break;
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		_type = "ERROR";
		break;

	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		_type = "DEPRECATED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		_type = "UDEFINED BEHAVIOR";
		break;

	case GL_DEBUG_TYPE_PORTABILITY:
		_type = "PORTABILITY";
		break;

	case GL_DEBUG_TYPE_PERFORMANCE:
		_type = "PERFORMANCE";
		break;

	case GL_DEBUG_TYPE_OTHER:
		_type = "OTHER";
		break;

	case GL_DEBUG_TYPE_MARKER:
		_type = "MARKER";
		break;

	default:
		_type = "UNKNOWN";
		break;
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		_severity = "HIGH";
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		_severity = "MEDIUM";
		break;

	case GL_DEBUG_SEVERITY_LOW:
		_severity = "LOW";
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		_severity = "NOTIFICATION";
		break;

	default:
		_severity = "UNKNOWN";
		break;
	}

	printf("%d: %s of %s severity, raised from %s: %s\n",
		id, _type, _severity, _source, message);
}