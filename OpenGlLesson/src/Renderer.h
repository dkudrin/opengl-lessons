#pragma once
#include <GLEW/include/GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak(); // Visual Studio specific breakpoint set
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLogCall(__FILE__, #x, __LINE__))


void GlClearError(); // функция очистки OpenGL от старых ошибок

bool GlLogCall(const char* fileName, const char* funcName, int codeLine); // функция вывода в цикле всех ошибок OpenGL
