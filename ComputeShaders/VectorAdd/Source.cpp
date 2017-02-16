#include<GL\glew.h>
#include<src\SOIL.h>
#include<GLFW\glfw3.h>
#include<thread>
#include<assert.h>
#include<iostream>
#include<vector>

#define GLSL(source) #source
#define NUM 12

const GLchar* cSrc = GLSL(
	#version 430 core \n
	layout(local_size_x = 2) in;
layout(std430, binding = 0) buffer A1{
	float A[];
};

layout(std430, binding = 1) buffer B1{
	float B[];
};


void main() {
	uint tx = gl_GlobalInvocationID.x;
	B[tx] = A[tx] + B[tx];
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

	GLuint cShader = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(cShader, 1, &cSrc, NULL);
	glCompileShader(cShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, cShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	std::vector<GLfloat> A(NUM);
	std::vector<GLfloat> B(NUM);
	for (int i = 0; i < NUM; i++) {
		A[i] = 3.0f;
		B[i] = 1.0f;
	}

	GLuint A_ssbo;
	glGenBuffers(1, &A_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, A_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM * sizeof(GLfloat), A.data(), GL_DYNAMIC_DRAW);


	GLuint B_ssbo;
	glGenBuffers(1, &B_ssbo);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, B_ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, NUM * sizeof(GLfloat), B.data(), GL_DYNAMIC_DRAW);

	glDispatchCompute(NUM / 2, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, B_ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, B_ssbo);

	GLfloat *ptr;
	ptr = (GLfloat*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

	memcpy(B.data(), ptr, NUM*sizeof(GLfloat));

	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

	for (unsigned int i = 0; i < NUM; i++) {
		std::cout << A[i] << " " << B[i] << std::endl;
	}

	glfwTerminate();
}