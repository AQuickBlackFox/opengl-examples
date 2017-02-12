#include<GL\glew.h>
#include<src\SOIL.h>
#include<GLFW\glfw3.h>
#include<thread>
#include<assert.h>
#include<iostream>

#define GLSL(source) #source

float vertices[] = {
	0.0f, 0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 1.0f,
};

float colors[] = {
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
};

const GLchar* vSrc = GLSL(
	#version 430 core \n
	layout(binding = 0) buffer vPos
	{
		vec2 position[];
	};
	layout(binding = 1) buffer pColor
	{
		float color[];
	};
	out float Color;
	void main(){
		gl_Position = vec4(position[gl_VertexID], 0.0f, 1.0f);
		Color = color[gl_VertexID];
	}
);

const GLchar* pSrc = GLSL(
	#version 430 core \n
	in float Color;
	void main(){
		gl_FragColor = vec4(Color, 0.0f, 0.0f, 1.0f);
	}
);

int main(){
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &vSrc, NULL);
	glCompileShader(vShader);

	GLuint pShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(pShader, 1, &pSrc, NULL);
	glCompileShader(pShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, pShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, vbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, vbo);

	GLuint pbo;
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, pbo);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	while (!glfwWindowShouldClose(window)){
		// Draw a triangle from the 3 vertices
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shaderProgram);
	glDeleteShader(vShader);
	glDeleteShader(pShader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &pbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
}