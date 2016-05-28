#ifndef QUAD_H
#define QUAD_H

#include <GL/glew.h>
#include <memory>
#include "shape.h"
#include "Shader.hpp"

class Quad : public Shape {
    public:
        Quad(float x = 0.00f, float y = 0.00f, float z = 0.00f);
        ~Quad() noexcept;
	/** Copy constructor*/
	Quad (const Quad& other);
	/** Copy assignment operator */
	Quad& operator= (const Quad& other);
	/** Move constructor*/
	Quad (Quad&& other) noexcept;
	/** Copy move operator */
	Quad& operator= (Quad&& other) noexcept;

        void setScaling(float x = 1.00f, float y = 1.0f, float z = 1.0f);
        void draw();
	int mvp_location, trans_location;

    private:
	void initShaders();
        float scaleFactorX, scaleFactorY, scaleFactorZ;
	GLfloat red, green, blue;
        static GLuint vbuffer, VertexArrayID, normalBuffer;
        GLuint shaderProgrammeID = 0;
        static std::unique_ptr<Shader> vertexShader, fragmentShader; 
};

#endif // QUAD_H
