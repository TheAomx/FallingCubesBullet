#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>

class Shader {
	public:
		Shader(std::string fileName, GLint type);
		GLuint  getID();

	private:
		GLuint shaderID;
		std::string name;
		char *shaderContent;
		static GLchar errorBuffer[1000];
		bool loadFile(const char *fileName);
};
#endif
