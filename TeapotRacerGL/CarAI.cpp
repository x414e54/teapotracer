#include "CarAI.h"
#include "AntCore/AntMath.h"
#include "AntCore/Ant.h"

//-----------------------------------------------------------------------------
// Update method
//-----------------------------------------------------------------------------
void CarAI::Update(float timeDelta)
{
	if (_game->IsPaused()) { return; }		// If game is paused return from update

	/* This is a completely stuPId AI, but does its job to keep the car on the board
		It is simply rule based, the direction the AI must go in is specified in each
		game board cell, from there the AI works out an ideal direction it should be facing
		and then will move along that, if it sences it is going to far off the edge of the board
		it will correct itsself.
		 
		To make the AI easier or harder you could adjust the amount that the AI corrects itseself, that
		way sometimes it may fall off if it is lower, or so on.
	*/

	float currentrot=_actor->GetOrientation();
	
	/* Checks the cells around, and the current cell */
	int* cell = _game->GetBoardCellAt(_actor->GetPosition()->_1, _actor->GetPosition()->_3);
	float x=cos(currentrot-(PI/2))*(2.0f);
	float z=sin(currentrot-(PI/2))*(-2.0f);

	int* cellLeft = _game->GetBoardCellAt(_actor->GetPosition()->_1+x, _actor->GetPosition()->_3+z);
	x=cos(currentrot+(PI/2))*(2.0f);			// This is the amount left or right that we check for our next cell, increasing this number will mean we check further
	z=sin(currentrot+(PI/2))*(-2.0f);			// therefore keePIng in a more central place in the current cell. We could store this in a view distance amount, and have it able to be changed.
	int* cellRight = _game->GetBoardCellAt(_actor->GetPosition()->_1+x, _actor->GetPosition()->_3+z);
	x=cos(currentrot)*+2.5f;
	z=sin(currentrot)*-2.5f;

	int* cellFront = _game->GetBoardCellAt(_actor->GetPosition()->_1+x, _actor->GetPosition()->_3+z);

	float idealrot=0.0f;

	// What cell are we currently on, what is our ideal direction?
	if (cell[1]=='>') {
		idealrot=0.0f;
	} else if (cell[1]=='+') {
		idealrot=-PI/4;
	} else if (cell[1]=='^') {
		idealrot=-PI/2;
	} else if (cell[1]=='<') {
		idealrot=-PI;
	} else if (cell[1]=='-') {
		idealrot=-PI/2+PI/4;
	} else if (cell[1]=='v') {
		idealrot=PI-PI/2;
	} else if (cell[1]=='o') {
		idealrot=-PI/2-PI/4;
	} else if (cell[1]=='d') {
		idealrot=PI-PI/4;
	} else if (cell[1]=='r') {
		idealrot=PI-PI/2+PI/4;
	} else if (cell[1]=='l') {
		idealrot=PI-PI/2-PI/4;
	}

	float angle = CalcAngle(currentrot, idealrot);
	if (angle < 0.0f)
	{
		_actor->SetRotation(-0.003f);
	} else if (angle > 0.0f	)
	{
		_actor->SetRotation(+0.003f);
	} else {
		_actor->SetRotation(0.0f);
	}

	// What sort is the cell infront of us? If its 0 or 7 we slow down.
	if (cellFront[0]==0||cellFront[0]==7) {
		_actor->SetThrottle(0.1f);
	} else {
		_actor->SetThrottle(0.4f);
	}

	// If the cell to our left or our right is 0, then we are too close to the edge, so turn a small amount.
	if (cellLeft[0]==0) {
		_actor->SetRotation(0.003f);
	} else if (cellRight[0]==0) {
		_actor->SetRotation(-0.003f);
	}

	_actor->SetGear(SLIDERCAR_GEAR_1);		// AI always goes forward.
}
