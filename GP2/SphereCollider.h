#include "BaseCollider.h"
#include "Physics.h"


//Sphere Collider
class CSphereCollider:public CBaseCollider
{
public:
	CSphereCollider();
	~CSphereCollider();

	//set and get radius
	void setRadius(float radius)
	{
		m_fRadius=radius;
	};
	float getRadius()
	{
		return m_fRadius;
	};
	
	//init method
	void init();
private:
	float m_fRadius;
};