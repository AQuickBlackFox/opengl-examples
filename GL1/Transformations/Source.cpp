#include<GL\glew.h>

#include<GLFW\glfw3.h>
#include<thread>
#include<chrono>

#include<glm\glm.hpp>
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtc\type_ptr.hpp>

#define GLSL(source) #source

const GLchar* vertexSource = GLSL(
	#version 150 core\n
	in vec2 position;
in vec3 color;
out vec3 Color;
uniform mat4 trans;
void main()
{
	Color = color;
	gl_Position = trans*vec4(position, 0.0, 1.0);
}
);

const GLchar* fragmentSource = GLSL(
	#version 150 core\n
	in vec3 Color;
out vec4 outColor;
void main()
{
	outColor = vec4(Color, 1.0);
}
);

GLuint elements[] = {
	0, 1, 2,
	2, 3, 0
};

GLfloat vertices[] = {
	-6.5f, 5.5f, 4.5f, 1.0f, 0.0f, 0.0f,
	6.5f, 5.5f, 4.5f, 1.0f, 0.0f, 0.0f, 
	6.5f, -5.5f, 4.5f, 1.0f, 0.0f, 0.0f,
	-6.5f, -5.5f, 4.5f, 1.0f, 0.0f, 0.0f,

	-7.5f, 6.5f, -4.5f, 0.0f, 1.0f, 0.0f,
	5.5f, 6.5f, -4.5f, 0.0f, 1.0f, 0.0f,
	5.5f, -4.5f, -4.5f, 0.0f, 1.0f, 0.0f,
	-7.5f, -4.5f, -4.5f, 0.0f, 1.0f, 0.0f,
};


GLuint indices[] = {
	0, 1, 2,
	2, 3, 0,

	4, 5, 6,
	6, 7, 4
};

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

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);

	GLuint ebo;
	glGenBuffers(1, &ebo);

/*	GLfloat vertices[] = {
		-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f
	};
*/
	glm::mat4 transform;
	transform[0] = glm::vec4(0.08f, 0.0f, 0.0f, 0.0f);
	transform[1] = glm::vec4(0.0f, 0.08f, 0.0f, 0.0f);
	transform[2] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

	glm::vec4 translate = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	transform[3] = translate;

	glm::vec4 scale;
	scale = glm::vec4(0.08f, 0.08f, 0.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	GLint uniTrans = glGetUniformLocation(shaderProgram, "trans");
	glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(transform));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 R[3]; // Rotate w.r.t X, Y, Z axis by 30 degrees


	R[0][0] = scale * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	R[0][1] = scale * glm::vec4(0.0f, cosf(glm::radians(30.0f)), -sinf(glm::radians(30.0f)), 0.0f);
	R[0][2] = scale * glm::vec4(0.0f, -sinf(glm::radians(30.0f)), cosf(glm::radians(30.0f)), 0.0f);
	R[0][3] = translate;

	R[1][0] = scale * glm::vec4(cosf(glm::radians(30.0f)), 0.0f, -sinf(glm::radians(30.0f)), 0.0f);
	R[1][0] = scale * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	R[1][0] = scale * glm::vec4(sinf(glm::radians(30.0f)), 0.0f, -cosf(glm::radians(30.0f)), 0.0f);
	R[1][3] = translate;

	R[2][0] = scale * glm::vec4(cosf(glm::radians(30.0f)), sinf(glm::radians(30.0f)), 0.0f, 0.0f);
	R[2][1] = scale * glm::vec4(-sinf(glm::radians(30.0f)), cosf(glm::radians(30.0f)), 0.0f, 0.0f);
	R[2][2] = scale * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
	R[2][3] = translate;

	uint32_t i = 0;

	while (!glfwWindowShouldClose(window)){

		if (i < 3){
			std::chrono::seconds sec(5);
			std::this_thread::sleep_for(sec);
			glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(R[i]));
			i++;
		}else{
			break;
		}
		// Draw a triangle from the 3 vertices
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
}