#pragma once
#include <GLEW/include/GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak(); // Visual Studio specific breakpoint set
#define GlCall(x) GlClearError();\
	x;\
	ASSERT(GlLogCall(__FILE__, #x, __LINE__))


void GlClearError(); // ������� ������� OpenGL �� ������ ������

bool GlLogCall(const char* fileName, const char* funcName, int codeLine); // ������� ������ � ����� ���� ������ OpenGL
