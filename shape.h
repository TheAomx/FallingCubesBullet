#ifndef SHAPE_H
#define SHAPE_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BulletDynamics/btBulletDynamicsCommon.h"
#include <GL/glew.h>

class Shape
{
  public:                              // öffentlich
    Shape(float x = 0.00f, float y = 0.00f, float z = 0.00f);
    ~Shape();                     // der Destruktor

    float getX();
    float getY();
    float getZ();

    void setPos(float x, float y, float z);

    float getSpeedX();
    float getSpeedY();
    float getSpeedZ();

    void setSpeed(float x = 0.00f, float y =0.0f, float z = 0.0f);

    float getRotateSpeedValueX();
    float getRotateSpeedValueY();
    float getRotateSpeedValueZ();
    void setRotateSpeed(float x = 0.0f, float y = 0.0f, float z = 0.0f);
    
    void setColor(float r, float g, float b);


    void setRotateValues(float rotX, float rotY, float rotZ);
    float getRotateValueX();
    float getRotateValueY();
    float getRotateValueZ();
    
    void setMatrix(btScalar *mat);

    virtual void draw() =  0;        // virtuelle Draw-Methode für jedes Shape
    virtual void update() = 0;        // virtuelle update-Methode für jedes Shape
    
    
    
    glm::mat4 ViewTranslate, ViewRotateX, ViewRotateY, ViewRotateZ, View, Model, MVP;
    GLuint colorBuffer;
    float color[3*24];
    static constexpr int matrix_dimension = 16;
    btScalar	m[matrix_dimension];
    bool mvp_changed;

  private:                             // privat
    
    void adjustMatrices();
    float x,y,z;
    float speedX, speedY, speedZ;
    float rotateX, rotateY, rotateZ;
    float rotateSpeedX, rotateSpeedY, rotateSpeedZ;
};


#endif // SHAPE_H
