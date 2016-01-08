#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <new>
#include "header.h"

GLchar Shader::errorBuffer[1000];

/**
 * Class that provides basic operation on GLSL Shaders. Shaders can be loaded from files and then later used in the program.
 * Note that errors may occur if a shader didnt compile well or a shader file doesnt exist.
 */
Shader::Shader(std::string fileName, GLint type) {
	GLint status = 0;

	this->name = fileName;
	loadFile(fileName.c_str());
	this->shaderID = glCreateShader(type);
	const char *p = this->shaderContent.c_str();
	glShaderSource(this->shaderID, 1, &p, NULL);

	glCompileShader(this->shaderID);
	glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) { // Problem beim Kompilieren aufgetaucht. Fehler ausgeben.
		int len = 0;
		glGetShaderInfoLog(this->shaderID, 1000, &len, Shader::errorBuffer);
		throw FatalException("compilation of " + name + " failed:\n  " + Shader::errorBuffer + "\n");
	}

	DBG("done loading file %s!", fileName.c_str());
}

GLuint Shader::getID() {
	return this->shaderID;
}

bool Shader::loadFile(const char *fileName) {
	std::ifstream in (fileName, std::ios::in | std::ios::binary);
	
	in.seekg(0, std::ios::end);
	shaderContent.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&shaderContent[0], shaderContent.size());
	
	return true;
}
