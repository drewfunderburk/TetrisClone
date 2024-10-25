#pragma once

class Shader
{
public:
	Shader(const char* filePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform4f(const char* name, float v0, float v1, float v2, float v3);


private:
	unsigned int LoadShader(const char* filePath);
	unsigned int CreateShader(const char* vertexShader, const char* fragmentShader);
	unsigned int CompileShader(unsigned int type, const char* source);

	unsigned int GetUniformLocation(const char* name);

private:
	const char* m_filePath;
	unsigned int m_rendererID;
};