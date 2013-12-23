#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>
#include "shape.h"
#include "Shader.hpp"

class Quad : public Shape {
    public:
        Quad(float x = 0.00f, float y = 0.00f, float z = 0.00f);
        ~Quad();                     // der Destruktor

        void setScaling(float x = 1.00f, float y = 1.0f, float z = 1.0f);
        void draw();
        void update();
    public:
        float scaleFactorX, scaleFactorY, scaleFactorZ;
        static GLuint vbuffer, VertexArrayID;
        GLuint shaderProgrammeID;
        static Shader *vertexShader,  *fragmentShader; 
        int mvp_location, trans_location, time_location;
//        MovementBehaviour *movement;

};

#endif // QUAD_H
