#include <stdio.h>
#include <memory>

#include "quad.h"
#include "header.h"
#include "Utils.h"

GLuint Quad::vbuffer = 0, Quad::VertexArrayID = 0;
//int Quad::mvp_location = 0, Quad::trans_location = 0;

std::unique_ptr<Shader> Quad::vertexShader = NULL;
std::unique_ptr<Shader> Quad::fragmentShader = NULL;

#define A -1,-1, 1, 
#define B  1,-1, 1, 
#define C -1, 1, 1,
#define D  1, 1, 1,
#define E -1,-1,-1,
#define F  1,-1,-1,
#define G -1, 1,-1,
#define H  1, 1,-1,

static const GLfloat cubeVerticesStrip[] = {
	// Front face
	A B C D
	// Right face
	B F H D
	// Back face
	F E H G
	// Left face
	E A G C
	// Bottom face
	E F A B
	// Top face
	C D G H
};

static const GLfloat cubeTexCoordsStrip[] = {
	// Front face
	0, 0, 1, 0, 0, 1, 1, 1,
	// Right face
	0, 0, 1, 0, 0, 1, 1, 1,
	// Back face
	0, 0, 1, 0, 0, 1, 1, 1,
	// Left face
	0, 0, 1, 0, 0, 1, 1, 1,
	// Bottom face
	0, 0, 1, 0, 0, 1, 1, 1,
	// Top face
	0, 0, 1, 0, 0, 1, 1, 1
};

#if 1
#if 0
#define A_NORMAL  1, 1, 1, 
#define B_NORMAL  1, -1, 1,
#define C_NORMAL  -1, 1, 1,
#define D_NORMAL  -1, -1, 1,
#define E_NORMAL  1, 1, -1, 
#define F_NORMAL  1, -1, -1, 
#define G_NORMAL  -1, 1, -1,
#define H_NORMAL  -1, -1, -1,
#endif

#define A_NORMAL  -1, -1, 1, 
#define B_NORMAL  1, -1, 1,
#define C_NORMAL  -1, 1, 1,
#define D_NORMAL  1, 1,  1,
#define E_NORMAL  -1, -1,-1, 
#define F_NORMAL  1, -1, -1, 
#define G_NORMAL  -1, 1, -1,
#define H_NORMAL  1, 1,  -1,

static const GLfloat cubeNormalsStrip [] = {
	// Front face
	A_NORMAL B_NORMAL C_NORMAL D_NORMAL
	// Right face
	B_NORMAL F_NORMAL H_NORMAL D_NORMAL
	// Back face
	F_NORMAL E_NORMAL H_NORMAL G_NORMAL
	// Left face
	E_NORMAL A_NORMAL G_NORMAL C_NORMAL
	// Bottom face
	E_NORMAL F_NORMAL A_NORMAL B_NORMAL
	// Top face
	C_NORMAL D_NORMAL G_NORMAL H_NORMAL
};
#endif

void Quad::initShaders() {
	glGenBuffers(1, &Quad::vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Quad::vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesStrip), &cubeVerticesStrip[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &Quad::VertexArrayID);
	glBindVertexArray(Quad::VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, Quad::vbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	glEnableVertexAttribArray(0);
	
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeNormalsStrip), cubeNormalsStrip, GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*) NULL // array buffer offset
	);

	Quad::vertexShader = std::make_unique<Shader>("quad.vertex", GL_VERTEX_SHADER);
	Quad::fragmentShader = std::make_unique<Shader>("quad.frag", GL_FRAGMENT_SHADER);
}

Quad::Quad(float x, float y, float z) {
	setPos(x, y, z);

	if (Quad::vbuffer == 0) {
		initShaders();
	}

	setColor(1.0f, 0.0f, 0.0f);

	shaderProgrammeID = glCreateProgram();
	glAttachShader(shaderProgrammeID, Quad::fragmentShader->getID());
	glAttachShader(shaderProgrammeID, Quad::vertexShader->getID());
	glLinkProgram(shaderProgrammeID);

	mvp_location = glGetUniformLocation(shaderProgrammeID, "mvp");
	glUseProgram(shaderProgrammeID);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));

	trans_location = glGetUniformLocation(shaderProgrammeID, "trans");

	this->scaleFactorX = 1.00f;
	this->scaleFactorY = 1.00f;
	this->scaleFactorZ = 1.0f;
	this->red = Utils::getRandNumber(0.0f, 1.0f);
	this->green = Utils::getRandNumber(0.0f, 1.0f);
	this->blue = Utils::getRandNumber(0.0f, 1.0f);
}

Quad::~Quad() {
 }

void Quad::setScaling(float x, float y, float z) {
	this->scaleFactorX = x;
	this->scaleFactorY = y;
	this->scaleFactorZ = z;
}

void Quad::draw() {
	glUseProgram(shaderProgrammeID);
	trans_location = glGetUniformLocation(shaderProgrammeID, "trans");
	glUniformMatrix4fv(trans_location, 1, GL_FALSE, this->m);
	
	glColor3f(this->red, this->green, this->blue);
	glBindVertexArray(Quad::VertexArrayID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(cubeVerticesStrip) / sizeof(float));
}

void Quad::update() {
	//printf("quad at x = %f, y = %f, z = %f\n", getX(), getY(), getZ());
	setRotateValues(getRotateValueX() + getRotateSpeedValueX(), getRotateValueY() + getRotateSpeedValueY(), getRotateValueZ() + getRotateSpeedValueZ());
	//    movement->move();
}
