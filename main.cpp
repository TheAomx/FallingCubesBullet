#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BulletDynamics/btBulletDynamicsCommon.h"
#include "header.h"
#include "Shader.hpp"
#include "Logger.h"
#include "shape.h"
#include "quad.h"
#include "camera.h"
#include "DynamicsWorld.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>



#if 1

#define NUM_QUADS 600

GLuint vbuffer, colorBuffer;
GLuint VertexArrayID;
Shader *vertexShader, *fragmentShader; 
GLuint shaderProgrammeID ;
int matrix_location, mvp_location = 0;
Quad *quads[NUM_QUADS];


int windowWidth = 1200, windowHeight = 900;

GLchar errorBuffer[1000];


/* Variablen von Bullet Physik Interface... */
DynamicsWorld myWorld;
btDynamicsWorld *m_dynamicsWorld;



Logger* Logger::instance = NULL;


GLFWwindow* window;

float posX = 0.0f;
float posY = 0.0f;

float posZ = -70.0;

#define ROT_SPEED 5.0f
#define MOVE_SPEED 2.0f


Camera camera;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if ((key==GLFW_KEY_W || key == GLFW_KEY_S || key == GLFW_KEY_RIGHT || key == GLFW_KEY_LEFT || key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_A || key == GLFW_KEY_D) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (key==GLFW_KEY_W || key==GLFW_KEY_S ) {
			
			if (key==GLFW_KEY_W) {
				camera.moveForward(MOVE_SPEED);
			}
			else {
				camera.moveForward(-MOVE_SPEED);
			}

        }
		else if (key==GLFW_KEY_A || key==GLFW_KEY_D) {
			
			if (key==GLFW_KEY_D) {
				camera.moveSideward(-MOVE_SPEED);
			} 		
			else {
				camera.moveSideward(MOVE_SPEED);
			}

		}

		else if(key == GLFW_KEY_RIGHT) {
			camera.incRotateY( ROT_SPEED );
		}
		else if (key == GLFW_KEY_LEFT) {
			camera.incRotateY( -1 * ROT_SPEED );
		}
		else if (key == GLFW_KEY_UP) {
			camera.incRotateX( -1 * ROT_SPEED );
		}
		else if (key == GLFW_KEY_DOWN) {
			camera.incRotateX(  ROT_SPEED );
		}

    }
}

void handleResize (GLFWwindow *window,int width,int height) {
    windowWidth = width;
    windowHeight = height;
    
	glViewport(0, 0, windowWidth, windowHeight);
	camera.setPerspective(45.0f, (float) windowWidth / (float) windowHeight, 0.1f, 200.0f);
}

void initPhysics() {
    
	myWorld.initWorld();
	m_dynamicsWorld = myWorld.getWorld();
    
    float mass = 1.0;
    
	for (int i = 0; i < NUM_QUADS; i++) {
		if (i <= 60) mass = 3.5f;
		else mass = 0.1f;
		Quad *myQuad = quads[i];
		myWorld.addRigidQuad(myQuad->getX(), myQuad->getY(),myQuad->getZ(), mass);
	}
}

void initializeOpenGL() {

	camera.setPerspective(45.0f, (float) windowWidth / (float) windowHeight, 0.1f, 200.0f);

    glEnable ( GL_DEPTH_TEST ); 
    double xVal = 0.0f;
    double yVal = 30.0f;
    double baseX = 0.00f;
    double t = 0.00;
    
    for (int i = 0; i < 60; i++) {
        if (i < 30)
            quads[i] = new Quad(xVal+1.0f+i*3.0f, 270.0f, 350.0f);
        else
            quads[i] = new Quad(xVal+1.0f+(i-20)*3.0f, 260.0f, 350.0f);
    }
    
    
    for (int i = 40; i < NUM_QUADS; i++) {
        t = -1 + 2 * ((float) (i / (float) NUM_QUADS));
        
        //        quads[i]->setColor(r,g,b);
        //        xVal = 5.00;
        if (i % 25 == 0) {
            xVal = 0.0f;
            yVal += 4.00f;
        }
        xVal += 3.5f;
        quads[i] = new Quad(xVal, yVal, 3.0f);
    }
    initPhysics();
   
        
        
//          glUniformMatrix4fv(Quad::mvp_location, 1, GL_FALE, glm::value_ptr(MVP));
	
}

void _update_fps_counter () {
  // timer/frame counter
  static double previous_seconds = glfwGetTime ();
  static int frame_count;
  double current_seconds = glfwGetTime ();
  double elapsed_seconds = current_seconds - previous_seconds;
  if (elapsed_seconds > 0.25) {
    previous_seconds = current_seconds;
    double fps = (double)frame_count / elapsed_seconds;
    char tmp[128];
     sprintf (tmp, "opengl @ fps: %.2lf", fps);
     glfwSetWindowTitle (window, tmp);
     frame_count = 0;
  }
  frame_count++;
}

int main(int argc, char** argv) {
	int running = GL_TRUE;
	  srand (time(NULL));

	/* Initialize GLFW */
	if( !glfwInit() )
	{
		exit( EXIT_FAILURE );
	}
        
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        DBG("hallo?");
	/*  Open an OpenGL window */
	window = glfwCreateWindow(windowWidth, windowHeight, "My Title", NULL, NULL);
	if( !window )
	{
		glfwTerminate();
                DBG("Window Creation failed");
		exit( EXIT_FAILURE );
	}
	
	glfwMakeContextCurrent(window);

	// Initialize GLEW

	if (glewInit() != GLEW_OK) {

		fprintf(stderr, "Failed to initialize GLEW\n");

		exit(EXIT_FAILURE);

	}

	
	
	glfwSetWindowTitle(window, "Shapes-C-Tryout");
	glfwSetKeyCallback(window, key_callback);
        glfwSetWindowSizeCallback(window, handleResize);
	/*  Main loop*/
	printf("OpenGL Version: %d %s\n",GL_VERSION, glGetString(GL_VERSION));
        

        int count = 0;
        GLFWmonitor *mainMonitor = glfwGetPrimaryMonitor();

        const GLFWvidmode *list =  glfwGetVideoModes(mainMonitor, &count);
        cout << "-------------------------------------------------------------------\n";
        cout << "Available Video Modes:\n";
        for (int i = 0; i < count; i++) {
          cout << list[i].width << "*" << list[i].height << " @ (" << list[i].redBits << "," << list[i].greenBits << "," << list[i].blueBits << ") RGB bits\n";
        }
        cout << "-------------------------------------------------------------------\n";

	
	
	initializeOpenGL();
        float speed = 1.0f; // move at 1 unit per second
        float last_position = 0.0f;
        
	while( !glfwWindowShouldClose(window) )
	{
                _update_fps_counter ();
                
                static double previous_seconds = glfwGetTime ();
                double current_seconds = glfwGetTime ();
                double elapsed_seconds = current_seconds - previous_seconds;
                previous_seconds = current_seconds;
                
                // reverse direction when going to far left or right
//                if (fabs(last_position) > 1.0f) {
//                  speed = -speed;
//                }

                // update the matrix
//                float angle = elapsed_seconds * speed + last_position;
//                createZRotMatrix(matrix, elapsed_seconds * speed + last_position);
//                last_position = angle;)
//                
//                glUseProgram (shaderProgrammeID);
//                glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
//                
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                //simple dynamics world doesn't handle fixed-time-stepping
                float ms = elapsed_seconds;

                ///step the simulation
                if (m_dynamicsWorld)
                {
                    for (int i = 0; i < 40; i++)
                        myWorld.getBody(i)->applyCentralForce(btVector3(btScalar(0.0f), btScalar(0.1f), btScalar(-5.00f)));
//                         printf("elapsed %f ms!\n", ms);
                        m_dynamicsWorld->stepSimulation(ms);
                        //optional but useful: debug drawing
//                        m_dynamicsWorld->debugDrawWorld();
                }
                
                for(int i=0;i<NUM_QUADS;i++) {
                    btCollisionObject *colObj=m_dynamicsWorld->getCollisionObjectArray()[i+1];
                    btRigidBody*		body=btRigidBody::upcast(colObj);
                    if (i == 0) body->applyCentralForce(btVector3(btScalar(0.0f), btScalar(0.0f), btScalar(0.01f)));
                    btScalar	m[16];
                    
                    if(body&&body->getMotionState())
                    {
                            btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
                            btVector3 origin = myMotionState->m_graphicsWorldTrans.getOrigin();
//                            quads[i]->setPos(origin.getX(), origin.getY(), origin.getZ());
                            myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
                            quads[i]->setMatrix(m);
//                            rot=myMotionState->m_graphicsWorldTrans.getBasis(initi);
                    }
                    else
                    {
//                        printf("blb");
                            colObj->getWorldTransform().getOpenGLMatrix(m);
                            quads[i]->setMatrix(m);
//                            rot=colObj->getWorldTransform().getBasis();
                    }
                }
                
//		glUseProgram (shaderProgrammeID);
//		glBindVertexArray (VertexArrayID);
//		glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
                glUseProgram (quads[0]->shaderProgrammeID);
                for (int i = 0; i < NUM_QUADS; i++) {
                    
                    glUniformMatrix4fv(quads[i]->mvp_location, 1, GL_FALSE, glm::value_ptr(camera.getMVP()));
                    quads[i]->draw();
                }
                
		/*  Swap front and back rendering buffers*/
		glfwSwapBuffers(window);
                glfwPollEvents();
		/*printf("drawed!\n");*/
		/*glfwSleep(0.001);*/
	}

	return 0;
}
#endif
