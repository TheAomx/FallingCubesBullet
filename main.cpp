#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>

#include "BulletDynamics/btBulletDynamicsCommon.h"
#include "header.h"
#include "Shader.hpp"
#include "Logger.h"
#include "shape.h"
#include "quad.h"
#include "camera.h"
#include "DynamicsWorld.h"

using namespace std;
#define ROT_SPEED 5.0f
#define MOVE_SPEED 2.0f
#define NUM_QUADS 500

int windowWidth = 1200, windowHeight = 900;

DynamicsWorld myWorld;
Logger* Logger::instance = NULL;
GLFWwindow* window;

Camera camera;
vector<Quad> quads;

static bool isKeyPressed(int *keysToCheck, int numKeys, int key) {
	for (int i = 0; i < numKeys; i++) {
		if (keysToCheck[i] == key) return true;
	}
	return false;
}

void addQuad(vector<Quad> &quads, float posX, float posY, float posZ, vec4 speedVec, float mass = 1.0f) {
	auto addedQuads = quads.size();
	quads.emplace_back(posX, posY, posZ);
	myWorld.addRigidQuad(btVector3(posX, posY, posZ), mass);
	myWorld.setSpeed(addedQuads, btVector3(speedVec.x, speedVec.y, speedVec.z));
}

void clearAllQuads(vector<Quad> &quads) {
	quads.clear();
	myWorld.clear();
}

void letQuadBlockFallDown() {
	constexpr double initial_y = 45.0f;
	constexpr double initial_x = -30.0f;
	constexpr double steps_x = 3.5f;
	constexpr double steps_y = 4.0f;
	double xVal = initial_x;
	double yVal = initial_y;
	float mass = 1.0f;
	vec4 vec = vec4(0.0f, -1.0f, 0.0f, 0.0f);

	for (int i = 0; i < NUM_QUADS; i++) {
		if (i % 10 == 0) {
			xVal += steps_x;
			yVal = initial_y;
			mass += 1.0f;
		}
		yVal -= steps_y;
		addQuad(quads, xVal, yVal, 3.0f, vec, mass);
	}
	
	DBG("letQuadBlockFallDown finished");
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static int CAMERA_KEYS[] = {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_RIGHT, GLFW_KEY_LEFT, 
	GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G,
	GLFW_KEY_SPACE};
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if ((isKeyPressed(CAMERA_KEYS, sizeof(CAMERA_KEYS) / sizeof(int), key)) && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
		switch (key) {
		case GLFW_KEY_W:
			camera.moveForward(-MOVE_SPEED);
			break;
		case GLFW_KEY_S:
			camera.moveForward(MOVE_SPEED);
			break;
		case GLFW_KEY_D:
			camera.moveSideward(MOVE_SPEED);
			break;
		case GLFW_KEY_A:
			camera.moveSideward(-MOVE_SPEED);
			break;
		case GLFW_KEY_RIGHT:
			camera.incRotateY(ROT_SPEED);
			break;
		case GLFW_KEY_LEFT:
			camera.incRotateY(-ROT_SPEED);
			break;
		case GLFW_KEY_UP:
			camera.incRotateX(-ROT_SPEED);
			break;
		case GLFW_KEY_DOWN:
			camera.incRotateX(ROT_SPEED);
			break;
		case GLFW_KEY_SPACE:
			clearAllQuads(quads);
			break;
		case GLFW_KEY_F:
		{
			vec4 posVec = vec4(0.0f, 0.0f, 3.0f, 0.0f) * camera.getViewRotateXY();
			posVec = posVec * -20.0f;
			DBG("adding at x=%f, y=%f, z=%f", camera.getX(), camera.getY(), camera.getZ());
			addQuad(quads, camera.getX(), camera.getY(), camera.getZ(), posVec);
		}
			break;
		case GLFW_KEY_G:
			letQuadBlockFallDown();
			break;
		}
	}
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
	DBG("mouse_callback called with button=%d, action=%d, mods=%d", button, action, mods);
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		double xpos, ypos;
		int width, height;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwGetWindowSize(window, &width, &height);
		double relX = xpos / (double) width;
		double relY = ypos / (double) height;
		
		vec4 posVec = vec4(0.0f, 0.0f, 3.0f, 0.0f) * camera.getViewRotateXY();
		posVec = posVec * -5.0f;
		
		DBG("mouse is at x: %f, y: %f, adding at x=%f, y=%f, z=%f",relX, relY, camera.getX(), camera.getY(), camera.getZ());
		addQuad(quads, camera.getX(), camera.getY(), camera.getZ(), posVec);
	}
}

void handleResize(GLFWwindow *window, int width, int height) {
	windowWidth = width;
	windowHeight = height;

	glViewport(0, 0, windowWidth, windowHeight);
	camera.setPerspective(45.0f, (float) windowWidth / (float) windowHeight, 0.1f, 400.0f);
}

void initPhysics() {
	myWorld.initWorld();
}

void initializeOpenGL() {
	camera.setPerspective(45.0f, (float) windowWidth / (float) windowHeight, 0.1f, 400.0f);

	glEnable(GL_DEPTH_TEST);
	//letQuadBlockFallDown();
}

void updateFPSCounter() {
	// timer/frame counter
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double) frame_count / elapsed_seconds;
		char tmp[128];
		sprintf(tmp, "opengl @ fps: %.2lf", fps);
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

void printOpenGLInfo() {
	printf("OpenGL Version: %d %s\n", GL_VERSION, glGetString(GL_VERSION));


	int count = 0;
	GLFWmonitor *mainMonitor = glfwGetPrimaryMonitor();

	const GLFWvidmode *list = glfwGetVideoModes(mainMonitor, &count);
	cout << "-------------------------------------------------------------------\n";
	cout << "Available Video Modes:\n";
	for (int i = 0; i < count; i++) {
		cout << list[i].width << "*" << list[i].height << " @ (" << list[i].redBits << "," << list[i].greenBits << "," << list[i].blueBits << ") RGB bits\n";
	}
	cout << "-------------------------------------------------------------------\n";
}

double getElapsedSeconds() {
	static double previous_seconds = glfwGetTime();
	double current_seconds = glfwGetTime();
	double elapsed_seconds = current_seconds - previous_seconds;
	previous_seconds = current_seconds;
	return elapsed_seconds;
}

int main(int argc, char** argv) {
	srand(time(NULL));

	/* Initialize GLFW */
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	//        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	/*  Open an OpenGL window */
	window = glfwCreateWindow(windowWidth, windowHeight, "My Title", NULL, NULL);
	if (!window) {
		glfwTerminate();
		DBG("Window Creation failed");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}

	glfwSetWindowTitle(window, "Shapes-C-Tryout");
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, handleResize);
	
	printOpenGLInfo();
	initPhysics();
	initializeOpenGL();

	/*  Main loop*/
	while (!glfwWindowShouldClose(window)) {
		updateFPSCounter();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		myWorld.stepSimulation(getElapsedSeconds());
		//for (int i = 0; i < 60; i++)
		//    myWorld.getBody(i)->applyCentralForce(btVector3(btScalar(0.0f), btScalar(0.1f), btScalar(-5.00f)));
		//

		//optional but useful: debug drawing
		//                        m_dynamicsWorld->debugDrawWorld();

		int i = 0;
		for (auto &quad : quads) {
			btScalar m[16];
			btRigidBody *body = myWorld.getBody(i);
			btDefaultMotionState* myMotionState = (btDefaultMotionState*) body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			quad.setMatrix(m);
			i++;
		}

		for (auto &quad : quads) {
			glUniformMatrix4fv(quad.mvp_location, 1, GL_FALSE, glm::value_ptr(camera.getMVP()));
			quad.draw();
		}

		/*  Swap front and back rendering buffers*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
