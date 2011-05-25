//*****************************************************************************
//-----------------------------------------------------------------------------
// PlayerControls.h 
//
// Class that allows a player to control a SliderCar by using the keyboard
//-----------------------------------------------------------------------------
//*****************************************************************************
#include "Windows.h"
#include "SliderCar.h"
#include "Game.h"

class PlayerControls
{
public:
	PlayerControls(Game* game);
	bool Event(UINT msg, WPARAM wParam, LPARAM lParam);
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