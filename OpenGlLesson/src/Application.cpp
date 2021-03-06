#include <GLEW/include/GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#pragma comment(lib,"user32.lib")

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};
	std::ifstream stream(filepath);
	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return {
		ss[0].str(),
		ss[1].str()
	};
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type); // returns ID of shader
	const char* src = source.c_str(); // ��������� ����� ������� � ������
	GlCall(glShaderSource(id, 1, &src, nullptr)); // �������� ������ ��� �������� ������
	GlCall(glCompileShader(id)); // ������������� ������ � ��������

	// TODO: add errors handling
	int result;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;
		std::cout << message << std::endl;
		GlCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
	unsigned int program = glCreateProgram(); // returns ID of shader program
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader); // vs - VertexShader
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader); // vs - FragmentShader
	GlCall(glAttachShader(program, vs));
	GlCall(glAttachShader(program, fs));

	GlCall(glLinkProgram(program));
	GlCall(glValidateProgram(program));
	
	GlCall(glDeleteShader(vs));
	GlCall(glDeleteShader(fs));

	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // ��������� ���� ��������� ���������� ���� frame ����� swap buffers - ���������� vertical synchronisation (vsync)

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error with glewInit!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{

		float positions[] = { // ������ ������� �������
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			-0.5f,  0.5f  // 3
		};

		unsigned int indices[] = { // ������ ������ ���� unsigned
			0, 1, 2,  // ������ �����������
			2, 3, 0   // ������ �����������
		};

		// �������� Vertex Array Object (vao)
		unsigned int vao;
		GlCall(glGenVertexArrays(1, &vao));
		GlCall(glBindVertexArray(vao));

		// �������� � ���������� ������� � ������ ����������
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		GlCall(glEnableVertexAttribArray(0)); // ����������� �������� 

		GlCall(glVertexAttribPointer(
			0, // ID of actual vao to them buffer will be bound. index. Specifies the index of the generic vertex attribute to be modified.
			2, // size. Specifies the number of components per generic vertex attribute. Must be 1, 2, 3, 4. 
			GL_FLOAT, // type. Specifies the data type of each component in the array.
			GL_FALSE, // normalized. specifies whether fixed-point data values should be normalized (GL_TRUE) or converted directly as fixed-point values (GL_FALSE) when they are accessed.
			sizeof(float) * 2, // stride. Specifies the byte offset between consecutive generic vertex attributes. If stride is 0, the generic vertex attributes are understood to be tightly packed in the array. The initial value is 0.
			(void*)0)); // pointer. Specifies a offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target. The initial value is 0.

		// �������� � ���������� ������� �������� � ������ ����������
		IndexBuffer ib(indices, 6);

		// �������� � ���������� ���������-�������
		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		std::cout << source.VertexSource << std::endl;
		std::cout << source.FragmentSource << std::endl;
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		GlCall(glUseProgram(shader));

		GlCall(int uniformLocation = glGetUniformLocation(shader, "u_Color")); // ��������� ����� � ������ ���������� u_Color ��������� � GLSL

		ASSERT(uniformLocation != -1); // ��������� ������ ������ ���� ��� ������ � ������

		GlCall(glUniform4f(uniformLocation, 0.2f, 0.3f, 0.8f, 1.0f)); // ���������� ������� � �����


		// Unbind everething
		GlCall(glBindVertexArray(0)); // �������� vertex array ������� - ���������
		GlCall(glUseProgram(0));
		GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // sets buffer ID to given free int ID (1)
		GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // sets buffer ID to given free int ID (2)



		float redChannel = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GlCall(glClear(GL_COLOR_BUFFER_BIT));

			GlCall(glUseProgram(shader));
			GlCall(glUniform4f(uniformLocation, redChannel, 0.3f, 0.8f, 1.0f)); // ���������� ������� � �����

			GlCall(glBindVertexArray(vao)); // �������� vertex array object (vao)
			ib.Bind(); // sets buffer ID to given free int ID (2)


			// ���������
			GlCall(glDrawElements(
				GL_TRIANGLES, // mode - Triangles, Specifies what kind of primitives to render. 
				(sizeof(indices) / sizeof(*indices)),  // count - indices array length - number of indices we draw
				GL_UNSIGNED_INT, // type Specifies the type of the values in indices.
				nullptr)); // we have allready bound ibo to the current GL_ELEMENT_ARRAY_BUFFER. Specifies an offset of the first index in the array in the data store of the buffer currently bound to the GL_ELEMENT_ARRAY_BUFFER target.			

			// Animate on red channel every frame
			if (redChannel > 1.0f)
				increment = -0.05f;
			else if (redChannel < 0.0f)
				increment = 0.05f;

			redChannel += increment;

			/* Swap front and back buffers */
			GlCall(glfwSwapBuffers(window));

			/* Poll for and process events */
			glfwPollEvents();
		}
		GlCall(glDeleteProgram(shader));
	}
	glfwTerminate(); // ����������� ��������� OpenGl
	return 0;
}