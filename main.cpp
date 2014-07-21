#include <GL/glew.h>
#include <GLFW/glfw3.h>
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

#define NUM_QUADS 800

GLuint vbuffer, colorBuffer;
GLuint VertexArrayID;
Shader *vertexShader, *fragmentShader; 
GLuint shaderProgrammeID ;
int matrix_location, mvp_location = 0;
int numReservedQuads = NUM_QUADS + 100;
int currentQuad = 0;
Quad **quads;


int windowWidth = 1200, windowHeight = 900;

GLchar errorBuffer[1000];


/* Variablen von Bullet Physik Interface... */
DynamicsWorld myWorld;
btDynamicsWorld *m_dynamicsWorld;

int numExtraQuads = 0;



Logger* Logger::instance = NULL;


GLFWwindow* window;

#define ROT_SPEED 5.0f
#define MOVE_SPEED 2.0f


Camera camera;

static bool isKeyPressed (int *keysToCheck, int numKeys, int key) {
	for (int i = 0; i < numKeys; i++) {
		if (keysToCheck[i] == key) return true;
	}
	return false;
}


void addQuad(float posX, float posY, float posZ, vec4 speedVec ) {
	if (currentQuad == numReservedQuads) {
		numReservedQuads += 100;
		quads = (Quad**) realloc(quads, sizeof(Quad*) * numReservedQuads);
	}


	
	quads[currentQuad] = new Quad(posX, posY, posZ);
	Quad *myQuad = quads[currentQuad];

	myWorld.addRigidQuad(myQuad->getX(), myQuad->getY(),myQuad->getZ(), 1.0f);	
	myWorld.setSpeed(currentQuad+1, speedVec.x, speedVec.y, speedVec.z);
	currentQuad++;

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	static int CAMERA_KEYS[] = {GLFW_KEY_W, GLFW_KEY_S , GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_A, GLFW_KEY_D,  GLFW_KEY_F };
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if ((isKeyPressed(CAMERA_KEYS, sizeof(CAMERA_KEYS) / sizeof(int), key)) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (key==GLFW_KEY_W) {
			camera.moveForward(-MOVE_SPEED);
        }
		else if (key==GLFW_KEY_S) {
			camera.moveForward(MOVE_SPEED);
		}
		else if (key==GLFW_KEY_D) {
				camera.moveSideward(MOVE_SPEED);
		}
		else if (key==GLFW_KEY_A) {
			camera.moveSideward(-MOVE_SPEED);
		}

		else if(key == GLFW_KEY_RIGHT) {
			camera.incRotateY( ROT_SPEED );
		}
		else if (key == GLFW_KEY_LEFT) {
			camera.incRotateY( -ROT_SPEED );
		}
		else if (key == GLFW_KEY_UP) {
			camera.incRotateX( -ROT_SPEED );
		}
		else if (key == GLFW_KEY_DOWN) {
			camera.incRotateX(  ROT_SPEED );
		}
		else if (key == GLFW_KEY_F) {
			
			vec4 posVec = vec4(0.0f, 0.0f, 5.0f, 0.0f) * camera.getViewRotateXY();
			posVec = posVec * -20.0f;
			DBG("adding at x=%f, y=%f, z=%f", camera.getX(), camera.getY(), camera.getZ());
			addQuad(camera.getX(), camera.getY(), camera.getZ(), posVec);	
			
			
		}

    }
}



void handleResize (GLFWwindow *window,int width,int height) {
    windowWidth = width;
    windowHeight = height;
    
	glViewport(0, 0, windowWidth, windowHeight);
	camera.setPerspective(45.0f, (float) windowWidth / (float) windowHeight, 0.1f, 400.0f);
}

void initPhysics() {
    
	myWorld.initWorld();
	m_dynamicsWorld = myWorld.getWorld();
}

void initializeOpenGL() {

	camera.setPerspective(45.0f, (float) windowWidth / (float) windowHeight, 0.1f, 400.0f);

    glEnable ( GL_DEPTH_TEST ); 
    double xVal = 0.0f;
    double yVal = 30.0f;
    double baseX = 0.00f;
    double t = 0.00;
	
	quads = (Quad**) malloc(sizeof(Quad*) * numReservedQuads);

	vec4 vec = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	
	addQuad(0.0f, -50.0f, 0.0f, vec );

    
    for (int i = 0; i < NUM_QUADS; i++) {
        t = -1 + 2 * ((float) (currentQuad / (float) NUM_QUADS));
        
        //        quads[i]->setColor(r,g,b);
        //        xVal = 5.00;
        if (i % 25 == 0) {
            xVal = 0.0f;
            yVal += 4.00f;
        }
        xVal += 3.5f;
		addQuad(xVal, yVal, 3.0f, vec);
    }

	
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

	
	initPhysics();
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

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//simple dynamics world doesn't handle fixed-time-stepping
		float ms = elapsed_seconds;

		///step the simulation
		if (m_dynamicsWorld)
		{
			//for (int i = 0; i < 60; i++)
			//    myWorld.getBody(i)->applyCentralForce(btVector3(btScalar(0.0f), btScalar(0.1f), btScalar(-5.00f)));
			//
			m_dynamicsWorld->stepSimulation(ms);
				//optional but useful: debug drawing
			//                        m_dynamicsWorld->debugDrawWorld();
		}


		btScalar	m[16];		

		for(int i=0;i<currentQuad-1;i++) {
			btRigidBody*		body =myWorld.getBody(i+2);
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();

			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			quads[i]->setMatrix(m);
		}

		glUseProgram (quads[0]->shaderProgrammeID);

		for (int i = 0; i < currentQuad; i++) {

			glUniformMatrix4fv(quads[i]->mvp_location, 1, GL_FALSE, glm::value_ptr(camera.getMVP()));
			quads[i]->draw();
		}
				
		/*  Swap front and back rendering buffers*/
		glfwSwapBuffers(window);
		glfwPollEvents();
		/*glfwSleep(0.001);*/
	}

	return 0;
}
#endif
