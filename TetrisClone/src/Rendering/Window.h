
class GLFWwindow;
class VertexBuffer;
class IndexBuffer;
class VertexArray;
class Shader;

class Window
{

public:
	Window(const char* title, int width, int height);
	~Window();

	void update();

	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	void setWindowSize(int width, int height);
	const char* getTitle() const { return m_title; }
	bool windowShouldClose();

	static void GLFWErrorMessageCallback(int error, const char* description);

private:
	bool init(const char* title, int width, int height);

private:
		GLFWwindow* m_window;
		int m_width;
		int m_height;
		const char* m_title;
		Shader* m_shader;
		unsigned int m_vertexArrayObject;
		VertexBuffer* m_vertexBuffer;
		IndexBuffer* m_indexBuffer;
		VertexArray* m_vertexArray;
};