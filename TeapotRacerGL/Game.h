//*****************************************************************************
//-----------------------------------------------------------------------------
// Game.h 
//
// Extended from AntGame.h
//
// Includes methods for adding objects, and players, and now has a board,
//
// The board consists for 10 by 10 cells and cells have a picture type and a direction
// for the AI to go in.
//-----------------------------------------------------------------------------
//*****************************************************************************
#pragma once
#ifndef GAME_H
#define GAME_H

#include "AntCore/AntGame.h"
#include "SliderCar.h"
#include "GameObject.h"
#include <vector>

class Game : public AntGame
{
public:
	Game();
	~Game();
	bool CleanUp();
	bool AddPlayer(unsigned int pID);
	bool AddObject(POVector3* pos, GAMEOBJECT_TYPE type);
	bool AddObject(POVector3* pos, POVector3* force, PointMass* target, GAMEOBJECT_TYPE type);
	void Update(float timeDelta);
	SliderCar* GetPlayer(int pID);
	int* GetBoardCell(int i, int j) { return _board[i][j]; }
	int GetNumberOfPlayers() { return _players.size(); }
	int GetNumberOfObjects() { return _objects.size(); }
	SliderCar** GetPlayers();
	GameObject** GetObjects();
	int* GetBoardCellAt(float x, float z);
	void LoadBoard();
	void ResetPlayers();
	void ResetPlayer(int pId);
	bool InterSectCheck(POSphere r1, POSphere r2);

protected:
	std::vector<SliderCar*> _players;
	std::vector<GameObject*> _objects;
	int _board[10][10][2];
};

#endif