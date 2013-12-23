#include "Shader.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <new>
#include "header.h"

GLchar Shader::errorBuffer[1000];

/**
 * Class that provides basic operation on GLSL Shaders. Shaders can be loaded from files and then later used in the program.
 * Note that errors may occur if a shader didnt compile well or a shader file doesnt exist.
 */
Shader::Shader(char *fileName, GLint type) {
	GLint status = 0;
	
	this->name = fileName;
        loadFile(fileName);
//        try {
//            loadFile(fileName);
//            
//        }
//	catch (FatalException &ex) {
//            ex.handleException();
//        }
	this->shaderID = glCreateShader (type);
	const char *p = this->shaderContent;
	glShaderSource (this->shaderID, 1,  &p , NULL);
	
	glCompileShader (this->shaderID);
	glGetShaderiv(this->shaderID, GL_COMPILE_STATUS, &status);
	
	if (status != GL_TRUE) {	// Problem beim Kompilieren aufgetaucht. Fehler ausgeben.
		int len = 0;
		glGetShaderInfoLog(this->shaderID, 1000, &len, Shader::errorBuffer);
                string exString = this->name;
		throw FatalException("compilation of "+exString+" failed:\n  "+Shader::errorBuffer+"\n");
	}	
	
	DBG("done loading file %s!", fileName);
	
	
}

GLuint Shader::getID() {
	return this->shaderID;
}

bool Shader::loadFile(char *fileName) {
	FILE *myFile;
	long lSize;
	size_t result;

	myFile = fopen ( fileName, "r" );
        string fname = fileName;
	if (myFile == NULL) throw FatalException("file " + fname + " could not be found!");
	 // obtain file size:
	fseek (myFile , 0 , SEEK_END);
	lSize = ftell (myFile);
	rewind (myFile);
	
	// allocate memory to contain the whole file:
	this->shaderContent = (char*) malloc (sizeof(char)*(lSize+1));
        if (this->shaderContent == NULL) throw new std::bad_alloc();
	memset(this->shaderContent, 0, sizeof(char)*(lSize+1));
	//if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	
	result = fread (this->shaderContent,1,lSize,myFile);
	this->shaderContent[lSize] = (char) 0;
		
	fclose(myFile);
	
	return true;

}