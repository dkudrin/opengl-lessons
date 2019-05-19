#include "Renderer.h"
#include <iostream>

void GlClearError() // ������� ������� OpenGL �� ������ ������
{
	while (glGetError() != GL_NO_ERROR); // ���� ��� ������ ��������� ��
}

bool GlLogCall(const char* fileName, const char* funcName, int codeLine) // ������� ������ � ����� ���� ������ OpenGL
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") File:" << fileName <<
			"; Function name:" << funcName << "; Codeline: " << codeLine << std::endl;
		return false;
	}
	return true;
}
