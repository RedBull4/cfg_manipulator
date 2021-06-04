#include "shaderLoader.hpp"

#include <glad/glad.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <string>

using namespace std;

namespace _namespace {
	void scanShaderForErrors(GLuint &shader, const GLuint &shaderType) {
		int success;
		char LOG[WCHAR_MAX];

		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success) {
			glGetShaderInfoLog(shader, WCHAR_MAX, NULL, LOG);

			if (shaderType == GL_VERTEX_SHADER)
				cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
					 << LOG << endl;
			else if (shaderType == GL_FRAGMENT_SHADER)
				cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
					 << LOG << endl;
		}
	}
}  // namespace _namespace

using namespace _namespace;

GLuint shaderLoader::loadShaders(const char *vertexShaderPath,
								 const char *fragmentShaderPath) {
	ifstream shaderFile;

	shaderFile.open(vertexShaderPath);

	if (!shaderFile) {
		cout << "Unable to open vertex shader file "s + string(vertexShaderPath)
					+ '.'
			 << endl;
		exit(EXIT_FAILURE);
	}

	string line, vertexShaderSource, fragmentShaderSource;

	while (getline(shaderFile, line)) { vertexShaderSource += line + '\n'; }

	shaderFile.close();
	shaderFile.clear();
	shaderFile.open(fragmentShaderPath);

	if (!shaderFile) {
		cout << "Unable to open fragment shader file "s
					+ string(vertexShaderPath) + '.'
			 << endl;
		exit(EXIT_FAILURE);
	}

	while (getline(shaderFile, line)) { fragmentShaderSource += line + '\n'; }

	shaderFile.close();
	shaderFile.clear();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER),
		   fragmentShader = glCreateShader(GL_FRAGMENT_SHADER),
		   shaderProgram = glCreateProgram();

	const char *vertexShaderSourceC_STR = vertexShaderSource.c_str(),
			   *fragmentShaderSourceC_STR = fragmentShaderSource.c_str();

	glShaderSource(vertexShader, 1, &vertexShaderSourceC_STR, NULL);
	glCompileShader(vertexShader);

	glShaderSource(fragmentShader, 1, &fragmentShaderSourceC_STR, NULL);
	glCompileShader(fragmentShader);

	scanShaderForErrors(vertexShader, GL_VERTEX_SHADER);
	scanShaderForErrors(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}