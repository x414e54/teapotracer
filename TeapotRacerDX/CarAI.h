//*****************************************************************************
//-----------------------------------------------------------------------------
// CarAI.h 
//
// Class that allows control of a SliderCar by simple AI
//-----------------------------------------------------------------------------
//*****************************************************************************
#include "SliderCar.h"
#include "Game.h"

class CarAI
{
public:
	CarAI(Game* game) : _game(game) {}
	void Update(float timeDelta);
	void SetActor(SliderCar* actor) { _actor=actor; }
protected:
	SliderCar* _actor;
	Game* _game;
};