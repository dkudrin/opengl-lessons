#include "Renderer.h"
#include <iostream>

void GlClearError() // функция очистки OpenGL от старых ошибок
{
	while (glGetError() != GL_NO_ERROR); // пока еть ошибки прочитать их
}

bool GlLogCall(const char* fileName, const char* funcName, int codeLine) // функция вывода в цикле всех ошибок OpenGL
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ") File:" << fileName <<
			"; Function name:" << funcName << "; Codeline: " << codeLine << std::endl;
		return false;
	}
	return true;
}
