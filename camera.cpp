#include "camera.h"

#include "header.h"

Camera::Camera() {
	rotX = -40.0f;
	rotY = 0.0f;	
	rotZ = 0.0f;
	setPos(0.0f, 0.0f, 70.0f);
	setPerspective(45.0f, 1.0f, 0.1f, 200.0f );
	
	recalcRotateView();
	Model = glm::scale(glm::mat4(1.0f),glm::vec3(1.0f));
	
	MVP = Projection * View * Model;
}

Camera::~Camera() {

}

void Camera::incRotateX(float incValue) {
	this->rotX += incValue;
	recalcRotateView();
}

void Camera::incRotateY(float incValue) {
	this->rotY += incValue;
	recalcRotateView();
}

void Camera::incRotateZ(float incValue) {
	this->rotZ += incValue;
	recalcRotateView();
}

void Camera::setRotateX (float rotX) {
	this->rotX = rotX;
	recalcRotateView();
}

void Camera::setRotateY (float rotY) {
	this->rotY = rotY;
	recalcRotateView();
}

void Camera::setRotateZ (float rotZ) {
	this->rotZ = rotZ;
	recalcRotateView();
}

void Camera::setPosX (float x) {
	this->posX = x;
	recalcView();
}

void Camera::setPosY (float y) {
	this->posY = y;
	recalcView();
}

void Camera::setPosZ (float z) {
	this->posZ = z;
	recalcView();
}

void Camera::setPos (float x, float y, float z) {
	this->posX = x;
	this->posY = y;
	this->posZ = z;
	recalcView();
}

float Camera::getX() {
	return this->posX;
}

float Camera::getY() {
	return this->posY;
}

float Camera::getZ() {
	return this->posZ;
}

void Camera::setPerspective(float angle, float ratio, float nearClip, float farClip) {
	Projection = glm::perspective(angle, ratio, nearClip, farClip);
	MVP = Projection * View * Model;
}

void Camera::recalcView() {
	View = glm::translate(ViewRotateXY,glm::vec3(-posX, -posY, -posZ));
	
	MVP = Projection * View * Model;
}

void Camera::recalcRotateView() {
	ViewRotateX = glm::rotate(glm::mat4(1.0f), glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	ViewRotateXY = glm::rotate(ViewRotateX, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	View = glm::translate(ViewRotateXY,glm::vec3(-posX, -posY, -posZ));
	
	MVP = Projection * View * Model;
}

void Camera::moveForward(float length) {
	vec4 posVec = vec4(0.0f, 0.0f, length, 0.0f) * this->getViewRotateXY();

	posX += posVec.x;
	posY += posVec.y;
	posZ += posVec.z;
	
	recalcView();
}
void Camera::moveSideward(float length) {
	vec4 posVec = vec4(length, 0.0f, 0.0f, 0.0f) * this->getViewRotateXY();
	
	posX += posVec.x;
	posY += posVec.y;
	posZ += posVec.z;
	
	recalcView();
}

mat4& Camera::getMVP() {
	return this->MVP;
}

mat4& Camera::getViewRotateXY() {
	return this->ViewRotateXY;
}
