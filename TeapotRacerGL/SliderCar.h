//*****************************************************************************
//-----------------------------------------------------------------------------
// SliderCar.h 
//
// Extended from AntActor.h
//
// This is an ant actor that acts like a car. The idea was to code it like it
// was sat ontop of a magnet, it can turn left or right by use of thrusters,
// And move forward and back, but applying a force.
// It also has a weapon which is enabled by picking up objects.
//-----------------------------------------------------------------------------
//*****************************************************************************
#pragma once
#ifndef SLIDERCAR_H
#define SLIDERCAR_H

#include "AntCore/AntActor.h"
#include "Weapon.h"
#include <sstream>

enum SLIDERCAR_GEAR
{
	SLIDERCAR_GEAR_REVERSE,
	SLIDERCAR_GEAR_PARK,
	SLIDERCAR_GEAR_1,
};

class SliderCar : public AntActor
{
public:
	SliderCar(unsigned int pId);
	~SliderCar();
	void SetBreak(bool breaking) { _breaking=breaking; }
	void SetThrottle(float throttle);
	void SetGear(SLIDERCAR_GEAR gear) { _gear=gear; }
	void SetRotation(float rot);
	void Boost() { if (_boostTimer<=0.0f) { _boostRequested=true; } }
	void SetFalling(bool falling) { _falling=falling; }
	void SetOrientation(float o);
	float GetThrottle() { return _throttle; }
	float GetRotation() { return _rot; }
	void Update(float timeDelta);
	void SetFloor(float floor) { _floor=floor; }
	std::wstring GetThrottleString();
	std::wstring GetSpeedString();
	void SetWeapon(WEAPON_TYPE type, int ammo) { _weapon->SetWeapon(type,ammo); }
	bool Fire() { return _weapon->Fire(); }
protected:
	float _boostTimer;
	bool _boostRequested;
	float _throttle;
	bool _falling;
	bool _breaking;
	SLIDERCAR_GEAR _gear;
	float _rot;
	float _floor;
	std::wstring _throttleStr;
	std::wstring _speedStr;
	Weapon* _weapon;
};

#endif
