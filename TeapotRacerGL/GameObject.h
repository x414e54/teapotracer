//*****************************************************************************
//-----------------------------------------------------------------------------
// GameObject.h 
//
// Extended from AntObject.h
//
// Class for a type of object that is not a player that will be rendered on a scene
// This is also the place for use of projectiles, that are fired from a player.
//-----------------------------------------------------------------------------
//*****************************************************************************
#include "AntCore/AntObject.h"
#include "AntCore/AntTimedEffect.h"

enum GAMEOBJECT_TYPE
{
	GAMEOBJECT_TYPE_BOOST,
	GAMEOBJECT_TYPE_SLOW,
	GAMEOBJECT_TYPE_ROCKET,
	GAMEOBJECT_TYPE_PROJECTILE,
};

class GameObject : public AntObject
{
public:
	GameObject(POVector3* pos, GAMEOBJECT_TYPE type, PointMass* _target);
	GameObject(POVector3* pos, GAMEOBJECT_TYPE type);
	~GameObject();
	void Update(float timeDelta);
	void PickUp() { _timedEffect->Start(); }
	bool IsSpawned() { return !_timedEffect->IsEnabled(); }
	GAMEOBJECT_TYPE GetType() { return _type; }

protected:
	GAMEOBJECT_TYPE _type;
	AntTimedEffect* _timedEffect;
	bool _spawned;
	PointMass* _target;
};