#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>

class Shader {
	public:
		Shader(char *fileName, GLint type);
		GLuint  getID();
	
	
	private:
		GLuint shaderID;
		char *name;
		char *shaderContent;
		static GLchar errorBuffer[1000];
		
		bool loadFile(char *fileName);

};
#endif