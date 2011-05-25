#include "Weapon.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
Weapon::Weapon()
{
	_type=WEAPON_TYPE_DISABLED;
	_charges=0;
	_cooldown=new AntTimedEffect();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
Weapon::~Weapon()
{
	delete _cooldown;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool Weapon::Fire()
{
	if (_type!=WEAPON_TYPE_DISABLED && _charges>0 && !_cooldown->IsEnabled()) 
	{
		_charges--;					// If weapon is not disabled reduce charges, start cooldown and return true
		_cooldown->Start();
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void Weapon::Update(float timeDelta)
{
	_cooldown->Update(timeDelta);
}