#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>
#include <memory>
#include "shape.h"
#include "Shader.hpp"

class Quad : public Shape {
    public:
        Quad(float x = 0.00f, float y = 0.00f, float z = 0.00f);
        ~Quad();                     // der Destruktor

        void setScaling(float x = 1.00f, float y = 1.0f, float z = 1.0f);
        void draw();
        void update();
	int mvp_location, trans_location, time_location;
	 
    private:
	void initShaders();
        float scaleFactorX, scaleFactorY, scaleFactorZ;
        static GLuint vbuffer, VertexArrayID;
        GLuint shaderProgrammeID;
        static std::unique_ptr<Shader> vertexShader, fragmentShader; 
};

#endif // QUAD_H
