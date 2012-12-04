#pragma once

#include <Common/Base/hkBase.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Monitor/hkMonitorStream.h>
#include <Common/Base/Monitor/MonitorStreamAnalyzer/hkMonitorStreamAnalyzer.h>
#include <Common/Base/Fwd/hkcstdio.h>

// Physics
#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Physics/Dynamics/Collide/ContactListener/hkpContactListener.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics/Collide/Shape/Convex/Sphere/hkpSphereShape.h>
#include <Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesConnectivityUtil.h>
#include <Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>

// Visual Debugger includes
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>

// Platform specific initialization
#include <Common/Base/System/Init/PlatformInit.cxx>

//Error function, this will get called if there is an error with Havok
static void HK_CALL errorReport(const char* msg, void* userContext);

//Physics Manager class
class CPhysics
{
public:
	//Static Get Instance
	static CPhysics& getInstance()
	{
		static CPhysics instance;
		return instance;
	};
	//Deconstructor
	~CPhysics();

	//Init method
	void init();
	//destroy
	void destroy();
	//update the physics simulation
	void update(float updateTime);

	//get the physics world
	hkpWorld * getPhysicsWorld()
	{
		return m_pWorld;
	};

	void setVisualDebug(bool debug)
	{
		m_bVisualDebug=debug;
	};
private:
	//hidden constructors
	CPhysics();
	CPhysics(CPhysics const&){};
	void operator=(CPhysics const&){}; 
private:
	bool m_bVisualDebug;
	//physic world
	hkpWorld* m_pWorld;
	//current update time
	float m_fCurrentTime;
	//Visual Debugger
	hkVisualDebugger* m_pVisualDebugger;
	hkpPhysicsContext* m_pContext;
};

