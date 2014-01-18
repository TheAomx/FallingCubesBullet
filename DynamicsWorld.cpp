#include "DynamicsWorld.h"

#define SCALING 2.0f


DynamicsWorld::DynamicsWorld () {
	cout << "dynamics world construktor" << endl;
}

DynamicsWorld::~DynamicsWorld () {
	delete m_collisionConfiguration;
	delete m_dispatcher;
	delete m_broadphase;
	delete m_solver;
	delete m_dynamicsWorld;
	
}

btDynamicsWorld* DynamicsWorld::getWorld () {
	return this->m_dynamicsWorld;
}

void DynamicsWorld::initWorld () {
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
	m_broadphase = new btDbvtBroadphase();

#ifdef PARALLEL
	m_threadSupportSolver = createSolverThreadSupport(MAX_TASKS);
	m_solver = new btParallelConstraintSolver(m_threadSupportSolver);
#else
	m_solver = new btSequentialImpulseConstraintSolver;
#endif
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	//    m_dynamicsWorld->setDebugDrawer(&gDebugDraw);

	m_dynamicsWorld->setGravity(btVector3(0,-1,0));
	
	//create a few basic rigid bodies
    btBoxShape *groundShape = new btBoxShape(btVector3(btScalar(150.0),btScalar(50.0),btScalar(150.0)));

	this->addShape(groundShape);
	
	this->addRigidShape(groundShape, 0.0f, -50.0f, 0.0f, 0.0f);

	{
        //create a few dynamic rigidbodies
        // Re-using the same collision is better for memory usage and performance
		colShape = new btBoxShape(btVector3(SCALING*0.5,SCALING*0.5,SCALING*0.5));
		 //btCollisionShape* colShape = new btSphereShape(btScalar(1.));
		this->addShape(colShape);
		startTransform.setIdentity();
		
	}
	
}

int DynamicsWorld::addShape (btCollisionShape *shape) {
	// TODO: what happens if push_backs fails? if it failed return -1 to indicate error...
	m_collisionShapes.push_back(shape);
	return 1;
}

int DynamicsWorld::addRigidBody(btRigidBody *body) {
	rigidBodies.push_back(body);
	m_dynamicsWorld->addRigidBody(body);
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
