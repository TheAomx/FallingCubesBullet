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
 
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>



#if 1

#define NUM_QUADS 500
#define SCALING 2.0f

GLuint vbuffer, colorBuffer;
GLuint VertexArrayID;
Shader *vertexShader, *fragmentShader; 
GLuint shaderProgrammeID ;
int matrix_location, mvp_location = 0;
Quad *quads[NUM_QUADS];
btRigidBody *body[NUM_QUADS];


int windowWidth = 1024, windowHeight = 768;

GLchar errorBuffer[1000];


/* Variablen von Bullet Physik Interface... */
btDefaultCollisionConfiguration *m_collisionConfiguration;
btCollisionDispatcher *m_dispatcher;
btBroadphaseInterface *m_broadphase;
btConstraintSolver *m_solver;
btDynamicsWorld *m_dynamicsWorld;
btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;



Logger* Logger::instance = NULL;


GLFWwindow* window;

static const float points[] = {
	-0.75f, -0.5f,  0.0f,
	0.25f,  -0.5f,  0.0f,
    -0.45f,  0.5f,  0.0f,
    0.55f,  0.5f,  0.0f,
 
  };

float colours[] = {
  1.0f, 0.0f,  0.0f,
  0.0f, 1.0f,  0.0f,
  0.0f, 0.0f,  1.0f,
  1.0f, 0.0f, 1.0f
};

float matrix[] = {
  1.0f, 0.0f, 0.0f, 0.0f, // first column
  0.0f, 1.0f, 0.0f, 0.0f, // second column
  0.0f, 0.0f, 1.0f, 0.0f, // third column
  0.0f, 0.3f, 0.0f, 1.0f // fourth column
};

float posZ = -40.0;

glm::mat4 Projection = glm::perspective(45.0f, (float)windowWidth/(float)windowHeight, 0.1f, 200.f);
glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, posZ));
glm::mat4 ViewRotateX = glm::rotate(ViewTranslate, 0.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
glm::mat4 View = glm::rotate(ViewRotateX, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Model = glm::scale(glm::mat4(1.0f),glm::vec3(0.5f));

glm::mat4 MVP = Projection * View * Model;

void createZRotMatrix(float *mat, float phi) {
    int i = 0;
    mat[i++] = cos(phi);
    mat[i++] = -sin(phi);
    mat[i++] = 0;
    mat[i++] = 0;
    mat[i++] = sin(phi);
    mat[i++] = cos(phi);
    mat[i++] = 0;
    mat[i++] = 0;
    mat[i++] = 0;
    mat[i++] = 0;
    mat[i++] = 1;
    mat[i++] = 0;
    mat[i++] = 0;
    mat[i++] = 0;
    mat[i++] = 0;
    mat[i++] = 1;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if ((key==GLFW_KEY_W || key == GLFW_KEY_S) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        if (key==GLFW_KEY_W ) {
            posZ += 0.5;
        }
        else if (key==GLFW_KEY_S) {
            posZ -= 0.5;
        }
        
        ViewTranslate = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f, 0.0f, posZ));
        ViewRotateX = glm::rotate(ViewTranslate, 0.0f, glm::vec3(-1.0f, 0.0f, 0.0f));
        View = glm::rotate(ViewRotateX, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        
         MVP = Projection * View * Model;
    }
     if ((key==GLFW_KEY_A || key == GLFW_KEY_D) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
         if (key==GLFW_KEY_A ) {
             for (int i = 0; i < NUM_QUADS; i++) {
                quads[i]->setRotateValues(quads[i]->getRotateValueX(), quads[i]->getRotateValueY()-10.0f, quads[i]->getRotateValueZ());
             }
        }
        else if (key==GLFW_KEY_D) {
            for (int i = 0; i < NUM_QUADS; i++) {
                quads[i]->setRotateValues(quads[i]->getRotateValueX(), quads[i]->getRotateValueY()+10.0f, quads[i]->getRotateValueZ());
             }
        }
     }
}

	
void handleKeys	(GLFWwindow *window,int keyID,int scanCode,int keyState,int mods) {
	if (keyID == GLFW_KEY_ESCAPE) {
		glfwTerminate();
		glDeleteBuffers(1, &vbuffer);
		glDeleteVertexArrays(1, &VertexArrayID);	
		exit(EXIT_SUCCESS);
	}
	printf("taste gedrückt! %d, %d\n", keyID, keyState);
}

void handleResize (GLFWwindow *window,int width,int height) {
    printf("resize callback called with width = %d, height = %d\n",width, height);
    windowWidth = width;
    windowHeight = height;
    
//    glfwSetWindowSize(window, width, height);
    
    Projection = glm::perspective(45.0f, (float) windowWidth / (float) windowHeight, 0.1f, 100.f);
    MVP = Projection * View * Model;
    
}

void initPhysics() {
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase = new btDbvtBroadphase();
    
    btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
    m_solver = sol;
    
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
//    m_dynamicsWorld->setDebugDrawer(&gDebugDraw);
	
    m_dynamicsWorld->setGravity(btVector3(0,-1,0));
    
    ///create a few basic rigid bodies
    btBoxShape *groundShape = new btBoxShape(btVector3(btScalar(150.0),btScalar(50.0),btScalar(150.0)));

    m_collisionShapes.push_back(groundShape);

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(0,-50,0));
    
    {
        btScalar mass(0.);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
                groundShape->calculateLocalInertia(mass,localInertia);

        //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        //add the body to the dynamics world
        m_dynamicsWorld->addRigidBody(body);
    }
    
    {
        //create a few dynamic rigidbodies
        // Re-using the same collision is better for memory usage and performance

        btBoxShape* colShape = new btBoxShape(btVector3(SCALING*0.5,SCALING*0.5,SCALING*0.5));
        //btCollisionShape* colShape = new btSphereShape(btScalar(1.));
        m_collisionShapes.push_back(colShape);

        /// Create Dynamic Objects
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar	mass(1.f);

        //rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
                colShape->calculateLocalInertia(mass,localInertia);

         for (int i = 0; i < NUM_QUADS; i++) {
             Quad *myQuad = quads[i];
             startTransform.setOrigin(btVector3(
                                                    btScalar(myQuad->getX()),
                                                    btScalar(myQuad->getY()),
                                                    btScalar(myQuad->getZ())));

            //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
            btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
            btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
            body[i] = new btRigidBody(rbInfo);
//            if (i == 0) body->applyCentralForce(btVector3(btScalar(0.0f), btScalar(0.0f), btScalar(1.00f)));


            m_dynamicsWorld->addRigidBody(body[i]);
         }
    }
}

void initializeOpenGL() {

	
	/* Vertex Buffer anlegen und anschließend an OpenGL binden (wg. State-Machine)
		anschließend werden die Buffer-Daten übergeben durch glBufferData. */
//	glGenBuffers (1, &vbuffer);
//	glBindBuffer (GL_ARRAY_BUFFER, vbuffer);
//	glBufferData (GL_ARRAY_BUFFER, sizeof (points), &points[0], GL_STATIC_DRAW);
//        
//        glGenBuffers (1, &colorBuffer);
//	glBindBuffer (GL_ARRAY_BUFFER, colorBuffer);
//	glBufferData (GL_ARRAY_BUFFER, sizeof (colours), &colours[0], GL_STATIC_DRAW);
//        
//	
//	glGenVertexArrays (1, &VertexArrayID);
//	glBindVertexArray (VertexArrayID);
//	glBindBuffer (GL_ARRAY_BUFFER, vbuffer);
//	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
//        glBindBuffer (GL_ARRAY_BUFFER, colorBuffer);
//	glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*)NULL);
//
//        glEnableVertexAttribArray (0);
//        glEnableVertexAttribArray (1);
//	
//	vertexShader = new Shader("example.vertex",  GL_VERTEX_SHADER);
//	fragmentShader = new Shader("example.frag",  GL_FRAGMENT_SHADER);
	
	
//	shaderProgrammeID = glCreateProgram ();
//	glAttachShader (shaderProgrammeID, fragmentShader->getID());
//	glAttachShader (shaderProgrammeID, vertexShader->getID());
//	glLinkProgram (shaderProgrammeID);
//                
//        createZRotMatrix(matrix, 0);
//        
//        matrix_location = glGetUniformLocation (shaderProgrammeID, "matrix");
//        glUseProgram (shaderProgrammeID);
//        glUniformMatrix4fv (matrix_location, 1, GL_FALSE, matrix);
//        
//        mvp_location = glGetUniformLocation (shaderProgrammeID, "mvp");
//        glUseProgram (shaderProgrammeID);
//        glUniformMatrix4fv (mvp_location, 1, GL_FALSE, matrix);

    glEnable ( GL_DEPTH_TEST ); 
    double xVal = 0.0f;
    double yVal = 30.0f;
    double baseX = 0.00f;
    double t = 0.00;
    
    for (int i = 0; i < 20; i++) {
        quads[i] = new Quad(xVal+1.0f+i*3.0f, 100.0f, 350.0f);
    }
    
    
    for (int i = 20; i < NUM_QUADS; i++) {
        t = -1 + 2 * ((float) (i / (float) NUM_QUADS));
        
        //        quads[i]->setColor(r,g,b);
        //        xVal = 5.00;
        if (i % 25 == 0) {
            yVal = 30.0f;
            xVal += 4.00f;
        }
        yVal += 3.5f;
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
                    for (int i = 0; i < 20; i++)
                        body[i]->applyCentralForce(btVector3(btScalar(0.0f), btScalar(0.1f), btScalar(-5.00f)));
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
                    
                    glUniformMatrix4fv(quads[i]->mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));
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
