#pragma once

class Shader
{
public:
	static unsigned int LoadShader(const char* filePath);
private:
	static unsigned int CreateShader(const char* vertexShader, const char* fragmentShader);
	static unsigned int CompileShader(unsigned int type, const char* source);
};