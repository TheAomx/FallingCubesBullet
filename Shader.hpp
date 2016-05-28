#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>

class Shader {
public:
	Shader(std::string fileName, GLint type);
	GLuint getID();

private:
	GLuint shaderID;
	std::string fileName;
	std::string shaderContent;
	static GLchar errorBuffer[1000];
	void loadFile();
};
#endif
