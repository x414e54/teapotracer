//*****************************************************************************
//-----------------------------------------------------------------------------
// Weapon.h 
//
// A class for Fireing a player's weapon and setting it's type, ammo and if it
// is enabled.
//-----------------------------------------------------------------------------
//*****************************************************************************
#include "AntCore/AntTimedEffect.h"

enum WEAPON_TYPE
{
	WEAPON_TYPE_LASER,
	WEAPON_TYPE_HOMING,
	WEAPON_TYPE_DISABLED,
};

class Weapon
{
public:
	Weapon();
	~Weapon();
	bool Fire();
	void Update(float timeDelta);
	void SetWeapon(WEAPON_TYPE type, int ammo) { _type=type; _charges=ammo; }

protected:
	AntTimedEffect* _cooldown;
	int _charges;
	WEAPON_TYPE _type;
};