#include "DynamicsWorld.h"

#define SCALING 2.0f


DynamicsWorld::DynamicsWorld () {
	cout << "dynamics world construktor" << endl;
}

DynamicsWorld::~DynamicsWorld () {
	delete dynamicsWorld;
}

btDynamicsWorld* DynamicsWorld::getWorld () {
	return this->dynamicsWorld;
}

void DynamicsWorld::initWorld () {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();

#ifdef PARALLEL
	threadSupportSolver = createSolverThreadSupport(MAX_TASKS);
	solver = new btParallelConstraintSolver(threadSupportSolver);
#else
	solver = new btSequentialImpulseConstraintSolver;
#endif
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
	//    m_dynamicsWorld->setDebugDrawer(&gDebugDraw);

	dynamicsWorld->setGravity(btVector3(0,-1,0));
	
	//create a few basic rigid bodies
    btBoxShape *groundShape = new btBoxShape(btVector3(btScalar(150.0),btScalar(50.0),btScalar(150.0)));

	this->addShape(groundShape);
	this->addRigidShape(groundShape, 0.0f, -50.0f, 0.0f, 0.0f);

    //create a few dynamic rigidbodies
    // Re-using the same collision is better for memory usage and performance
	colShape = new btBoxShape(btVector3(SCALING*0.5,SCALING*0.5,SCALING*0.5));
	 //btCollisionShape* colShape = new btSphereShape(btScalar(1.));
	this->addShape(colShape);
	startTransform.setIdentity();
}

int DynamicsWorld::addShape (btCollisionShape *shape) {
	// TODO: what happens if push_backs fails? if it failed return -1 to indicate error...
	collisionShapes.push_back(shape);
	return 1;
}

int DynamicsWorld::addRigidBody(btRigidBody *body) {
	rigidBodies.push_back(body);
	dynamicsWorld->addRigidBody(body);
	return 1;
}

btRigidBody* DynamicsWorld::getBody(int i) {
	return rigidBodies.at(i);
}

int DynamicsWorld::addRigidShape(btCollisionShape *shape, float x, float y, float z, float mass) {
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0,0,0);
	
	if (isDynamic) 
    	shape->calculateLocalInertia(mass,localInertia);

	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(btScalar(x),
                                       btScalar(y),
                                       btScalar(z)));
	
	 //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
    btRigidBody *b = new btRigidBody(rbInfo);
	
	this->addRigidBody(b);
	return 1;
} 

void DynamicsWorld::setSpeed(int i, float speedX, float speedY, float speedZ) {
	rigidBodies.at(i)->setLinearVelocity(btVector3(speedX, speedY, speedZ));
}

int DynamicsWorld::addRigidQuad(float x, float y, float z, float mass) {
	return this->addRigidShape(colShape, x, y, z, mass);
}
