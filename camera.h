#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Camera {
	public:
	Camera();
	~Camera();
	
	void incRotateX(float incValue);
	void incRotateY(float incValue);
	void incRotateZ(float incValue);
	
	void setRotateX(float rotX);
	void setRotateY(float rotY);
	void setRotateZ(float rotZ);

	void setPosX(float x);
	void setPosY(float y);
	void setPosZ(float z);
	
	void setPos(float x, float y, float z);
		
	mat4& getViewRotateXY();
	void setPerspective(float angle, float ratio, float nearClip, float farClip);
	
	void moveForward(float length);
	void moveSideward(float length);
	
	float getX();
	float getY();
	float getZ();
	
	mat4& getMVP();

	private:
	
	mat4 Projection;
	mat4 ViewRotateX;
	mat4 ViewRotateXY;
	mat4 View;
	mat4 Model;	
	
	mat4 MVP;
	
	float rotX, rotY, rotZ;
	
	float posX, posY, posZ;
	
	void recalcView();
	void recalcRotateView();
	
};

#endif
