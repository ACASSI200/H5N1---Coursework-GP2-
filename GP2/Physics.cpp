#include "Physics.h"

// Keycode
#include <Common/Base/keycode.cxx>

// Productfeatures
// We're using only physics - we undef products even if the keycode is present so
// that we don't get the usual initialization for these products.
#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR

// Also we're not using any serialization/versioning so we don't need any of these.
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
//#define HK_EXCLUDE_FEATURE_RegisterReflectedClasses
#define HK_EXCLUDE_FEATURE_MemoryTracker

// This include generates an initialization function based on the products
// and the excluded features.
#include <Common/Base/Config/hkProductFeatures.cxx>

//Error function
static void HK_CALL errorReport(const char* msg, void* userContext)
{
	OutputDebugStringA(msg);
}

//Update time for physics
const float PhysicsUpdateTime=0.0166f;

CPhysics::CPhysics()
{
	m_bVisualDebug=false;
	m_pWorld=NULL;
	m_pVisualDebugger=NULL;
	m_pContext=NULL;
}

CPhysics::~CPhysics()
{
}

void CPhysics::init()
{
	// Need to have memory allocated for the solver. Allocate 1mb for it.
	hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, 
		hkMemorySystem::FrameInfo(1024*1024) );
	//Init base physics system
	hkBaseSystem::init( memoryRouter, errorReport );
	//Setup physics world
	hkpWorldCinfo info;
	//gravity
	info.m_gravity.set( 0,-9.8f,0);
	//broad phase world 
	info.setBroadPhaseWorldSize( 1500.0f );
	//Physics world creation
	m_pWorld = new hkpWorld( info);
	// Register all collision agents
	// It's important to register collision agents before adding any entities to the world.
	hkpAgentRegisterUtil::registerAllAgents( m_pWorld->getCollisionDispatcher() );

	if (m_bVisualDebug){
		// Register all the physics viewers
		hkpPhysicsContext::registerAllPhysicsProcesses(); 

		// Set up a physics context containing the world for the use in the visual debugger
		m_pContext = new hkpPhysicsContext;
		m_pContext->addWorld(m_pWorld); 
	
		hkArray<hkProcessContext*> contexts;
		contexts.pushBack(m_pContext);
		m_pVisualDebugger = new hkVisualDebugger(contexts);
		m_pVisualDebugger->serve();
	}
}

//Update method
void CPhysics::update(float updateTime)
{
	//Update time
	m_fCurrentTime+=updateTime;
	//if this time is above physics time
	if (m_fCurrentTime>PhysicsUpdateTime)
	{
		//set the current time to zero
		m_fCurrentTime=0.0f;
		//step the physics simulation
		m_pWorld->stepDeltaTime(PhysicsUpdateTime);
		//Update visual debugger
		//if (m_bVisualDebug)
		//	m_pVisualDebugger->step();
	}
	
}

//destroy
void CPhysics::destroy()
{
	if (m_bVisualDebug){
		//Remove visual debugger
		m_pVisualDebugger->removeReference();
	}
	//remove the world
	m_pWorld->removeReference();
	
	if (m_bVisualDebug){
		//remove context
		m_pContext->removeReference();
	}
	//exit the base system and memory
	hkBaseSystem::quit();
	hkMemoryInitUtil::quit();
}