#include<GL\glew.h>
#include<src\SOIL.h>
#include<GLFW\glfw3.h>
#include<thread>
#include<assert.h>
#include<iostream>

#define GLSL(source) #source

#define XDim 16
#define YDim 16

static const GLchar* vShaderSrc = GLSL(
	#version 150 core \n
	in float in_pos;
	out float out_pos;
	void main() {
		out_pos = in_pos;
	}
);

int main(){
	
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &vShaderSrc, nullptr);
	glCompileShader(shader);
	GLuint program = glCreateProgram();
	glAttachShader(program, shader);

	const GLchar* feedbackVaryings[] = { "out_pos" };
	glTransformFeedbackVaryings(program, 1, feedbackVaryings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(program);
	glUseProgram(program);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	GLint inputAttrib = glGetAttribLocation(program, "in_pos");
	glEnableVertexAttribArray(inputAttrib);
	glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint tbo;
	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);

	glEnable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);

	glBeginTransformFeedback(GL_POINTS);

	glDrawArrays(GL_POINTS, 0, 5);
	glEndTransformFeedback();
	glFlush();
	GLfloat feedback[5];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

	
	for (int i = 0; i < 5; i++)
	{
		std::cout << feedback[i] << std::endl;
	}
	glDeleteProgram(program);
	glDeleteShader(shader);

	glDeleteBuffers(1, &tbo);
	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
}