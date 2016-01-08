#ifndef DYNAMICS_WORLD_H
#define DYNAMICS_WORLD_H

#include <iostream>
#include <vector>
#include "BulletDynamics/btBulletDynamicsCommon.h"

#if 0
#define PARALLEL 1
#define MAX_TASKS 4

#include "BulletMultiThreaded/SpuGatheringCollisionDispatcher.h"
#include "BulletMultiThreaded/PlatformDefinitions.h"

#include "BulletMultiThreaded/PosixThreadSupport.h"
#include "BulletMultiThreaded/SpuNarrowPhaseCollisionTask/SpuGatheringCollisionTask.h"

#include "BulletMultiThreaded/btParallelConstraintSolver.h"
#include "BulletMultiThreaded/SequentialThreadSupport.h"

class	btThreadSupportInterface;

#endif

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
		int addRigidShape(btCollisionShape *shape, btVector3 pos, float mass = 1.0f);
		int addRigidQuad(btVector3 pos, float mass = 1.0f); 
		void setSpeed(int i, btVector3 speed);
		void stepSimulation (float ms);

    private:
		btRigidBody* createRigidBody(btCollisionShape *shape, btVector3 pos, float mass);
		btDynamicsWorld *dynamicsWorld;		
#ifdef PARALLEL
		class	btThreadSupportInterface *threadSupportSolver;
		btThreadSupportInterface* createSolverThreadSupport(int maxNumThreads)
		{

			PosixThreadSupport::ThreadConstructionInfo solverConstructionInfo("solver", SolverThreadFunc,
																			  SolverlsMemoryFunc, maxNumThreads);
	
			PosixThreadSupport* threadSupport = new PosixThreadSupport(solverConstructionInfo);

			return threadSupport;
		}
#endif
		btDefaultCollisionConfiguration *collisionConfiguration;
		btCollisionDispatcher *dispatcher;
		btBroadphaseInterface *broadphase;
		btConstraintSolver *solver;	
		btBoxShape* colShape;
		btTransform startTransform;
		btAlignedObjectArray<btCollisionShape*> collisionShapes;
		vector<btRigidBody*> rigidBodies;
};

#endif // DYNAMICS_WORLD_H
