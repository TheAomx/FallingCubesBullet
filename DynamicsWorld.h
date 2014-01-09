#ifndef DYNAMICS_WORLD_H
#define DYNAMICS_WORLD_H

#include <iostream>
#include <vector>
#include "BulletDynamics/btBulletDynamicsCommon.h"

using namespace std;

class DynamicsWorld {
    public:
        DynamicsWorld();
        ~DynamicsWorld();                     // der Destruktor

		btDynamicsWorld* getWorld();
		void initWorld();
		int addShape (btCollisionShape *shape);
		int addRigidBody(btRigidBody *body);
		btRigidBody* getBody(int i);
		int addRigidShape(btCollisionShape *shape, float x, float y, float z, float mass = 1.0f);
		int addRigidQuad(float x, float y, float z, float mass = 1.0f);

    private:
		btDynamicsWorld *m_dynamicsWorld;		
		btDefaultCollisionConfiguration *m_collisionConfiguration;
		btCollisionDispatcher *m_dispatcher;
		btBroadphaseInterface *m_broadphase;
		btConstraintSolver *m_solver;	
		btBoxShape* colShape;
		btTransform startTransform;
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;
		vector<btRigidBody*> rigidBodies;     
		

};

#endif // DYNAMICS_WORLD_H
