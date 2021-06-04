#include "cfg_manipulator.hpp"
#include "shaderLoader.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <string.h>
#include <string>

using namespace std;

struct windowSettings {
	glm::i32vec2 size, position;
	const char *title;
} windowSettings;

GLFWwindow *window = NULL;

cfg_manipulator::cfg_file settings;

const char *vertexShaderPath, *fragmentShaderPath;

void centerWindowPosition() {
	const GLFWvidmode *videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	windowSettings.position
		= {videoMode->width / 2 - windowSettings.size.x / 2,
		   videoMode->height / 2 - windowSettings.size.y / 2};
}

void initWindowSettings() {
	windowSettings.size = {atoi(settings.read("window", "width")),
						   atoi(settings.read("window", "height"))};
	windowSettings.title = settings.read("window", "title");

	if (strcmp(settings.read("window", "position"), "CENTERED") == 0) {
		centerWindowPosition();
	}
}

void printWindowSettings() {
	cout << "window_size = {" + to_string(windowSettings.size.x) + ", "s
				+ to_string(windowSettings.size.y) + "}\n";
	cout << "window_position = {" + to_string(windowSettings.position.x) + ", "
				+ to_string(windowSettings.position.y) + "}\n";
	cout << "window_title = \""s + windowSettings.title + "\"\n";
}

int main() {
	if (glfwInit() == GLFW_FALSE) {
		cout << "Error: Unable to initialize GLFW library.";
		exit(EXIT_FAILURE);
	}

	settings.open("settings.cfg");

	initWindowSettings();

	printWindowSettings();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowSettings.size.x, windowSettings.size.y,
							  windowSettings.title, NULL, NULL);

	if (window == NULL) {
		cout << "Error: Unable to create main window" << endl;
		exit(EXIT_FAILURE);
	}

	glfwSetWindowPos(window, windowSettings.position.x,
					 windowSettings.position.y);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	vertexShaderPath = settings.read("shaders", "vertexShader");
	fragmentShaderPath = settings.read("shaders", "fragmentShader");

	GLuint shaderProgram
		= shaderLoader::loadShaders(vertexShaderPath, fragmentShaderPath);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,	 // left
		0.5f,  -0.5f, 0.0f,	 // right
		0.0f,  0.5f,  0.0f	 // top
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	do {
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (!glfwWindowShouldClose(window));

	glfwDestroyWindow(window);

	glfwTerminate();
}