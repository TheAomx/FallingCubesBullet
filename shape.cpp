

#include "shape.h"
#include "header.h"

Shape::Shape(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->mvp_changed = false;
	setRotateValues(0.00f, 0.00f, 0.00f);
	setSpeed();
	setRotateSpeed();
}

Shape::~Shape() {
 }
// der Destruktor

void Shape::setRotateValues(float rotX, float rotY, float rotZ) {
	this->rotateX = rotX;
	this->rotateY = rotY;
	this->rotateZ = rotZ;

	//    DBG("rotateX = %f, rotateY = %f, rotateZ = %f", rotateX, rotateY, rotateZ);

	ViewRotateX = glm::rotate(ViewTranslate, glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f));
	ViewRotateY = glm::rotate(ViewRotateX, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f));
	ViewRotateZ = glm::rotate(ViewRotateY, glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f));
	adjustMatrices();
}

void Shape::setMatrix(btScalar *mat) {
	// TODO: why does std::copy not work in this case?	
	//std::copy(&m[0], &m[15], mat);

	for (int i = 0; i < 16; i++) {
		this->m[i] = mat[i];
	}
}

float Shape::getRotateValueX() {
	return this->rotateX;
}

float Shape::getRotateValueY() {
	return this->rotateY;
}

float Shape::getRotateValueZ() {
	return this->rotateZ;
}

float Shape::getX() {
	return this->x;
}

float Shape::getY() {
	return this->y;
}

float Shape::getZ() {
	return this->z;
}

void Shape::setPos(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;

	ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
	setRotateValues(getRotateValueX(), getRotateValueY(), getRotateValueZ());
	adjustMatrices();
}

float Shape::getSpeedX() {

	return this->speedX;
}

float Shape::getSpeedY() {
	return this->speedY;

}

float Shape::getSpeedZ() {
	return this->speedZ;
}

void Shape::setSpeed(float x, float y, float z) {
	this->speedX = x;
	this->speedY = y;
	this->speedZ = z;
}

float Shape::getRotateSpeedValueX() {
	return this->rotateSpeedX;

}

float Shape::getRotateSpeedValueY() {
	return this->rotateSpeedY;
}

float Shape::getRotateSpeedValueZ() {
	return this->rotateSpeedZ;

}

void Shape::setRotateSpeed(float x, float y, float z) {
	this->rotateSpeedX = x;
	this->rotateSpeedY = y;
	this->rotateSpeedZ = z;
}

void Shape::adjustMatrices() {
	//    View = glm::rotate(ViewRotateZ, 20.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	View = ViewRotateZ;
	Model = glm::mat4(1.0f);

	MVP = View * Model;
	this->mvp_changed = true;
}

void Shape::setColor(float r, float g, float b) {
	for (int v = 0; v < 24; v++) {
		color[3 * v + 0] = r;
		color[3 * v + 1] = g;
		color[3 * v + 2] = b;
	}
}
