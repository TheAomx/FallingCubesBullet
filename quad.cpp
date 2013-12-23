#include "quad.h"

#include <stdio.h>

#include "header.h"

GLuint Quad::vbuffer = 0, Quad::VertexArrayID = 0;
//int Quad::mvp_location = 0, Quad::trans_location = 0;

Shader* Quad::vertexShader = NULL;
Shader* Quad::fragmentShader = NULL;

static const float cubeVerticesStrip[] = {
    // Front face
    -1,-1,1, 1,-1,1, -1,1,1, 1,1,1,
    // Right face
    1,-1,1, 1,-1,-1, 1,1,1, 1,1,-1,
    // Back face
    1,-1,-1, -1,-1,-1, 1,1,-1, -1,1,-1,
    // Left face
    -1,-1,-1, -1,-1,1, -1,1,-1, -1,1,1,
    // Bottom face
    -1,-1,-1, 1,-1,-1, -1,-1,1, 1,-1,1,
    // Top face
    -1,1,1, 1,1,1, -1,1,-1, 1,1,-1
};

 static const float cubeTexCoordsStrip[] = {
        // Front face
        0,0, 1,0, 0,1, 1,1,
        // Right face
        0,0, 1,0, 0,1, 1,1,
        // Back face
        0,0, 1,0, 0,1, 1,1,
        // Left face
        0,0, 1,0, 0,1, 1,1,
        // Bottom face
        0,0, 1,0, 0,1, 1,1,
        // Top face
        0,0, 1,0, 0,1, 1,1
    };

Quad::Quad(float x, float y, float z) {
    setPos(x,y,z);
    
   
    
    
    
    if (Quad::vbuffer == 0) {
        glGenBuffers (1, &Quad::vbuffer);
	glBindBuffer (GL_ARRAY_BUFFER, Quad::vbuffer);
	glBufferData (GL_ARRAY_BUFFER, sizeof (cubeVerticesStrip), &cubeVerticesStrip[0], GL_STATIC_DRAW);
        
        glGenVertexArrays (1, &Quad::VertexArrayID);
	glBindVertexArray (Quad::VertexArrayID);
	glBindBuffer (GL_ARRAY_BUFFER, Quad::vbuffer);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
        
        glEnableVertexAttribArray (0);
       
        
        Quad::vertexShader = new Shader("quad.vertex",  GL_VERTEX_SHADER);
        Quad::fragmentShader = new Shader("quad.frag",  GL_FRAGMENT_SHADER);
        
        
    }
    
    glGenBuffers (1, &colorBuffer);
    glBindBuffer (GL_ARRAY_BUFFER, colorBuffer);
    glBufferData (GL_ARRAY_BUFFER, sizeof (this->color), &this->color[0], GL_STATIC_DRAW);
    
    
    float r = (float)rand()/(float)RAND_MAX;
    float g = (float)rand()/(float)RAND_MAX;
    float b = (float)rand()/(float)RAND_MAX;
    
    DBG("r=%f,g=%f,b=%f",r,g,b);
    
    setColor(1.0f, 0.0f, 0.0f);
    
    
    glBindBuffer (GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
    
    glEnableVertexAttribArray (1);
    
    shaderProgrammeID = glCreateProgram ();
    glAttachShader (shaderProgrammeID, Quad::fragmentShader->getID());
    glAttachShader (shaderProgrammeID, Quad::vertexShader->getID());
    glLinkProgram (shaderProgrammeID);

    mvp_location = glGetUniformLocation (shaderProgrammeID, "mvp");
    glUseProgram (shaderProgrammeID);
    glUniformMatrix4fv (mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));
    
    time_location = glGetUniformLocation (shaderProgrammeID, "time");
    
     
    glUniform1f(time_location, (float) rand()/(float)RAND_MAX);

    DBG("mvp was %d", mvp_location);

//    trans_location = glGetUniformLocation (shaderProgrammeID, "trans");
//    glUniformMatrix4fv (trans_location, 1, GL_FALSE, glm::value_ptr(this->MVP));
        
        DBG("trans was %d",  trans_location );

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
    
    glUseProgram (Quad::shaderProgrammeID);
//    if (this->mvp_changed) {
        trans_location = glGetUniformLocation (shaderProgrammeID, "trans");
        glUniformMatrix4fv (trans_location, 1, GL_FALSE, this->m);
//        this->mvp_changed = false;
//    }
        
    glBindVertexArray (Quad::VertexArrayID);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, sizeof(cubeVerticesStrip)/sizeof(float));

//    glPushMatrix();
//    glTranslatef(getX(), getY(), getZ());
//    glRotatef(getRotateValueZ(), 0.00, 0.00, 1.00);
//    glRotatef(getRotateValueY(), 0.00f, 1.00f, 0.00f);
//    glRotatef(getRotateValueX(),1.00f, 0.00, 0.00);
//    glScalef(this->scaleFactorX, this->scaleFactorY, this->scaleFactorZ);

//    glBegin(GL_QUADS);
//
//
//      glColor3f(0.0f,0.5f,0.0f); 
//      glNormal3f(0.0, 1.0f, 0.0f);
//      glVertex3f( 0.5f, 0.5f,-0.5f); // oben rechts (OBEN)
//      glVertex3f(-0.5f, 0.5f,-0.5f); // oben links (OBEN)
//      glVertex3f(-0.5f, 0.5f, 0.5f); // unten links (OBEN)
//      glVertex3f( 0.5f, 0.5f, 0.5f); // unten rechts  (OBEN)
//
//       glColor3f(0.5f,0.5f,0.0f); 
//       glNormal3f(0.0, -1.0f, 0.0f);
//       glVertex3f( 0.5f,-0.5f, 0.5f); // oben rechts (UNTEN)
//       glVertex3f(-0.5f,-0.5f, 0.5f); // oben links (UNTEN)
//       glVertex3f(-0.5f,-0.5f,-0.5f); // unten links (UNTEN)
//       glVertex3f( 0.5f,-0.5f,-0.5f); // unten rechts  (UNTEN)
//
//       glColor3f(0.5f,0.0f,0.0f); 
//       glNormal3f(0.0, 0.0f, 1.0f);
//       glVertex3f( 0.5f, 0.5f, 0.5f); // oben rechts (VORNE)
//       glVertex3f(-0.5f, 0.5f, 0.5f); // oben links (VORNE)
//       glVertex3f(-0.5f,-0.5f, 0.5f); // unten links (VORNE)
//       glVertex3f( 0.5f,-0.5f, 0.5f); // unten rechts  (VORNE)
//
//       glColor3f(0.5f,0.5f,0.0f); 
//       glNormal3f(0.0, 0.0f, -1.0f);
//       glVertex3f( 0.5f,-0.5f,-0.5f); // oben rechts (HINTEN)
//       glVertex3f(-0.5f,-0.5f,-0.5f); // oben links (HINTEN)
//       glVertex3f(-0.5f, 0.5f,-0.5f); // unten links (HINTEN)
//       glVertex3f( 0.5f, 0.5f,-0.5f); // unten rechts  (HINTEN)
//
//       glColor3f(0.5f,0.5f,0.5f); 
//       glNormal3f(-1.0, 0.0f, 0.0f);
//       glVertex3f(-0.5f, 0.5f, 0.5f); // oben rechts (LINKS)
//       glVertex3f(-0.5f, 0.5f,-0.5f); // oben links (LINKS)
//       glVertex3f(-0.5f,-0.5f,-0.5f); // unten links (LINKS)
//       glVertex3f(-0.5f,-0.5f, 0.5f); // unten rechts  (LINKS)
//
//       glColor3f(0.5f,0.0f,0.5f); 
//       glNormal3f(1.0, 0.0f, 0.0f);
//       glVertex3f( 0.5f, 0.5f,-0.5f); // oben rechts (RECHTS)
//       glVertex3f( 0.5f, 0.5f, 0.5f); // oben links (RECHTS)
//       glVertex3f( 0.5f,-0.5f, 0.5f); // unten links (RECHTS)
//       glVertex3f( 0.5f,-0.5f,-0.5f); // unten rechts (RECHTS)
//    glEnd();
//
//    glPopMatrix();

}

void Quad::update() {
    //printf("quad at x = %f, y = %f, z = %f\n", getX(), getY(), getZ());
    setRotateValues(getRotateValueX()+getRotateSpeedValueX(), getRotateValueY()+getRotateSpeedValueY(),getRotateValueZ()+getRotateSpeedValueZ());
//    movement->move();
}
