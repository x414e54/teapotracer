#include "SliderCar.h"
#include "AntCore/AntMath.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
SliderCar::SliderCar(unsigned int pId)
{
	_throttle=0.0f;
	_gear=SLIDERCAR_GEAR_PARK;
	_rot=0.0f;
	_orientation=0.0f;
	_meshId=0;
	_center.SetMass(1.0f);
	_center.SetPosition(0.0f,0.0f,0.0f);
	_breaking=false;
	_floor=2.0f;
	_boostTimer=0.0f;
	_boostRequested=false;
	_falling=false;
	_pId=pId;
	_radius=1.0f;
	_weapon=new Weapon();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
SliderCar::~SliderCar()
{
	delete _weapon;
	_weapon=NULL;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void SliderCar::SetThrottle(float throttle)
{
	if (throttle<0.0f) { _throttle=0.0f; }
	else if (throttle>1.0f) { _throttle=1.0f; }
	else { _throttle=throttle; }
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void SliderCar::SetRotation(float rot)
{
	if (rot<-1.0f) { _rot=-1.0f; }
	else if (rot>1.0f) { _rot=1.0f; }
	else { _rot=rot; }
}

//-----------------------------------------------------------------------------
// Update method for a slider car
//-----------------------------------------------------------------------------
void SliderCar::Update(float timeDelta)
{

	/* If a boost was requsted set the timeout on the boost and then, apply some force */
	if (_boostTimer<=0.0f)
	{ 
		if (_boostRequested)
		{
			float x=cos(_orientation)*0.1f;
			float z=sin(_orientation)*-0.1f;
			_center.AddForce(POVector3(x,0.0f,z));
			_boostTimer=10000.0f;
			_boostRequested=false;
		} else {
			_boostTimer=0.0f;
		}
	} else
	{
		_boostTimer-=timeDelta;
	}


	float startdepth=_floor;
	float maxdepth=1.0f;
	float constant =0.0009f;
	float depth= _center.GetPosition()->_2;

	//Car Bob - simple method of floating the car simulating it is on a magnet and creating engine judder
	if (depth < startdepth && !_falling)
	{
		POVector3 force;
		if (depth <= startdepth - maxdepth)
		{
			force._2=constant;
		} else {
			force._2=constant*(depth-startdepth-maxdepth)/2*maxdepth;
		}
		_center.AddForce(force);
	} else {
		_center.AddForce(POVector3(0.0f, -0.0009f, 0.0f));
	}

	/* Rotate our car in the direction requested */
	_orientation+=_rot;

	if (_orientation>PI) {
		_orientation=-(PI-(_orientation-PI));
	} else if (_orientation<-PI) {
		_orientation=(PI-(_orientation-PI));
	}
 
	if (!_falling) {			// If the car is falling then we do not process this
	if (_breaking)
	{
		_center.AddForce(POVector3(0.000002f*-_center._velocity._1,0.0f,0.000002f*-_center._velocity._3)); // Apply break fiction
	} else
	{
		switch(_gear)
		{
		case SLIDERCAR_GEAR_1:				// Fowards
			if (_throttle>0.0f)
			{
				float x=cos(_orientation)*_throttle/5000.0f;
				float z=sin(_orientation)*-_throttle/5000.0f;
				_center.AddForce(POVector3(x,0.0f,z));
			}
		break;
		case SLIDERCAR_GEAR_REVERSE:		// Backwards
			if (_throttle>0.0f)
			{
				float x=cos(_orientation)*-_throttle/10000.0f;
				float z=sin(_orientation)*_throttle/10000.0f;
				_center.AddForce(POVector3(x,0.0f,z));
			}
		break;
		}
	}
	}
	//if (_center._velocity._3>0.000001f) {
	//	if(_center._velocity._3<0.001f) {
	//		_center.AddForce(POVector3(0.0f,0.0f,-_center._velocity._3));
		//} else {
			//_center.AddForce(POVector3(0.0f,0.0f,0.001f));
		//}
		//_center.AddForce(POVector3(,0.0f,1.0f));
	//}
	//_center.AddForce(POVector3(,0.0f,1.0f));


	_center.Calculate(timeDelta);			// Update the center

	_center.AddForce(POVector3(0.007f*-_center._velocity._1,0.02f*-_center._velocity._2,0.007f*-_center._velocity._3)); // Apply some drag to the car

	_weapon->Update(timeDelta);				// Update the weapon
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
LPCWSTR SliderCar::GetThrottleString()
{
	std::wstringstream w;
	w<<_throttle;
	w>>_throttleStr;
	return _throttleStr.c_str();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
LPCWSTR SliderCar::GetSpeedString()
{
	std::wstringstream w;
	w<<Magnitude(&_center._velocity);
	w>>_speedStr;
	return _speedStr.c_str();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void SliderCar::SetOrientation(float or)
{
	_orientation=or;
}