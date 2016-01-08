#include "DynamicsWorld.h"

#define SCALING 2.0f

DynamicsWorld::DynamicsWorld() {
	cout << "dynamics world construktor" << endl;
}

DynamicsWorld::~DynamicsWorld() {
	delete dynamicsWorld;
}

btDynamicsWorld* DynamicsWorld::getWorld() {
	return this->dynamicsWorld;
}

void DynamicsWorld::initWorld() {
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphase = new btDbvtBroadphase();

#ifdef PARALLEL
	threadSupportSolver = createSolverThreadSupport(MAX_TASKS);
	solver = new btParallelConstraintSolver(threadSupportSolver);
#else
	solver = new btSequentialImpulseConstraintSolver;
#endif
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	//    m_dynamicsWorld->setDebugDrawer(&gDebugDraw);

	dynamicsWorld->setGravity(btVector3(0, -0.1, 0));
	startTransform.setIdentity();

	//create a few basic rigid bodies
	btBoxShape *groundShape = new btBoxShape(btVector3(btScalar(150.0), btScalar(50.0), btScalar(150.0)));
	btRigidBody *groundRigidBody = createRigidBody(groundShape, btVector3(0.0f, -50.0f, 0.0f), 0.0f);
	dynamicsWorld->addRigidBody(groundRigidBody);

	//create a few dynamic rigidbodies
	// Re-using the same collision is better for memory usage and performance
	colShape = new btBoxShape(btVector3(SCALING * 0.5, SCALING * 0.5, SCALING * 0.5));
	//btCollisionShape* colShape = new btSphereShape(btScalar(1.));

}

int DynamicsWorld::addShape(btCollisionShape *shape) {
	return 1;
}

btRigidBody* DynamicsWorld::getBody(int i) {
	return rigidBodies.at(i);
}

btRigidBody* DynamicsWorld::createRigidBody(btCollisionShape *shape, btVector3 pos, float mass) {
	bool isDynamic = (mass != 0.f);
	btVector3 localInertia(0, 0, 0);

	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	startTransform.setIdentity();
	startTransform.setOrigin(pos);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, shape, localInertia);
	btRigidBody *b = new btRigidBody(rbInfo);
	return b;
}

int DynamicsWorld::addRigidBody(btRigidBody *body) {
	rigidBodies.push_back(body);
	dynamicsWorld->addRigidBody(body);
	return 1;
}

int DynamicsWorld::addRigidShape(btCollisionShape *shape, btVector3 pos, float mass) {
	btRigidBody *b = createRigidBody(shape, pos, mass);
	this->addRigidBody(b);
	return 1;
}

void DynamicsWorld::setSpeed(int i, btVector3 speed) {
	rigidBodies.at(i)->setLinearVelocity(speed);
}

int DynamicsWorld::addRigidQuad(btVector3 pos, float mass) {
	return this->addRigidShape(colShape, pos, mass);
}

void DynamicsWorld::stepSimulation(float ms) {
	dynamicsWorld->stepSimulation(ms);
}

void DynamicsWorld::clear() {
	for (auto body : rigidBodies) {
		dynamicsWorld->removeRigidBody(body);
		delete body;
	}
	rigidBodies.clear();
}