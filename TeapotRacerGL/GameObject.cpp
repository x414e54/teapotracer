#include "GameObject.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
GameObject::GameObject(POVector3* pos, GAMEOBJECT_TYPE type) : AntObject(pos) {
	_timedEffect= new AntTimedEffect(10000.0f);
	_type=type;
	_radius=0.5f;
	_orientation=0.0f;
	_target=0;
	_center.SetMass(1.0f);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
GameObject::GameObject(POVector3* pos, GAMEOBJECT_TYPE type, PointMass* target) : AntObject(pos) {
	_timedEffect= new AntTimedEffect(10000.0f);
	_type=type;
	_radius=0.5f;
	_orientation=0.0f;
	_target=target;
	_center.SetMass(1.0f);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
GameObject::~GameObject()
{
	delete _timedEffect;
}

//-----------------------------------------------------------------------------
// Update method
//-----------------------------------------------------------------------------
void GameObject::Update(float timeDelta)
{
	/* Update the timed effect */
	_timedEffect->Update(timeDelta);

	if (_type==GAMEOBJECT_TYPE_PROJECTILE)
	{
		_center.Calculate(timeDelta);			// If it is a projectile then update it's pointMass
	} else
	{
		SetOrientation(GetOrientation()+(timeDelta/500.0f));		// If its any other object make it rotate.
	}
}