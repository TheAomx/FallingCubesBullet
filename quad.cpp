#include <stdio.h>
#include <memory>

#include "quad.h"
#include "header.h"

GLuint Quad::vbuffer = 0, Quad::VertexArrayID = 0;
//int Quad::mvp_location = 0, Quad::trans_location = 0;

std::unique_ptr<Shader> Quad::vertexShader = NULL;
std::unique_ptr<Shader> Quad::fragmentShader = NULL;

static const float cubeVerticesStrip[] = {
	// Front face
	-1, -1, 1, 1, -1, 1, -1, 1, 1, 1, 1, 1,
	// Right face
	1, -1, 1, 1, -1, -1, 1, 1, 1, 1, 1, -1,
	// Back face
	1, -1, -1, -1, -1, -1, 1, 1, -1, -1, 1, -1,
	// Left face
	-1, -1, -1, -1, -1, 1, -1, 1, -1, -1, 1, 1,
	// Bottom face
	-1, -1, -1, 1, -1, -1, -1, -1, 1, 1, -1, 1,
	// Top face
	-1, 1, 1, 1, 1, 1, -1, 1, -1, 1, 1, -1
};

static const float cubeTexCoordsStrip[] = {
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

void Quad::initShaders() {
	glGenBuffers(1, &Quad::vbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, Quad::vbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesStrip), &cubeVerticesStrip[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &Quad::VertexArrayID);
	glBindVertexArray(Quad::VertexArrayID);
	glBindBuffer(GL_ARRAY_BUFFER, Quad::vbuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	glEnableVertexAttribArray(0);

	Quad::vertexShader = std::make_unique<Shader>("quad.vertex", GL_VERTEX_SHADER);
	Quad::fragmentShader = std::make_unique<Shader>("quad.frag", GL_FRAGMENT_SHADER);
}

Quad::Quad(float x, float y, float z) {
	setPos(x, y, z);

	if (Quad::vbuffer == 0) {
		initShaders();
	}

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->color), &this->color[0], GL_STATIC_DRAW);

	setColor(1.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	glEnableVertexAttribArray(1);

	shaderProgrammeID = glCreateProgram();
	glAttachShader(shaderProgrammeID, Quad::fragmentShader->getID());
	glAttachShader(shaderProgrammeID, Quad::vertexShader->getID());
	glLinkProgram(shaderProgrammeID);

	mvp_location = glGetUniformLocation(shaderProgrammeID, "mvp");
	glUseProgram(shaderProgrammeID);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));

	time_location = glGetUniformLocation(shaderProgrammeID, "time");
	glUniform1f(time_location, (float) rand() / (float) RAND_MAX);


	trans_location = glGetUniformLocation(shaderProgrammeID, "trans");
	//    glUniformMatrix4fv (trans_location, 1, GL_FALSE, glm::value_ptr(this->MVP));

	this->scaleFactorX = 1.00f;
	this->scaleFactorY = 1.00f;
	this->scaleFactorZ = 1.0f;
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

	glBindVertexArray(Quad::VertexArrayID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, sizeof(cubeVerticesStrip) / sizeof(float));
}

void Quad::update() {
	//printf("quad at x = %f, y = %f, z = %f\n", getX(), getY(), getZ());
	setRotateValues(getRotateValueX() + getRotateSpeedValueX(), getRotateValueY() + getRotateSpeedValueY(), getRotateValueZ() + getRotateSpeedValueZ());
	//    movement->move();
}
