//*****************************************************************************
//-----------------------------------------------------------------------------
// PlayerControls.h 
//
// Class that allows a player to control a SliderCar by using the keyboard
//-----------------------------------------------------------------------------
//*****************************************************************************
#include "SliderCar.h"
#include "Game.h"
#include "AntCore/Ant.h"
class PlayerControls
{
public:
	PlayerControls(Game* game);
	bool Event(UINT uMsg, UINT p1, UINT p2);
	void SetActor(SliderCar* actor) { _actor=actor; }
	void Update(float timeDelta);
protected:
	SliderCar* _actor;
	bool _forward;
	bool _back;
	bool _left;
	bool _right;
	bool _break;
	Game* _game;
};
