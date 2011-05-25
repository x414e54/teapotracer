#include "Game.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
Game::Game()
{
	LoadBoard();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
Game::~Game()
{
	CleanUp();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool Game::CleanUp()
{
	for (unsigned int i=0; i<_players.size(); i++)
	{
		delete _players.at(i);
	}
	_players.clear();

	for (unsigned int i=0; i<_objects.size(); i++)
	{
		delete _objects.at(i);
	}
	_objects.clear();
	return true;
}


//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
SliderCar* Game::GetPlayer(int pID)
{
	for (unsigned int i=0; i<_players.size(); i++)
	{
		if (_players.at(i)->GetpId()==pID) { return _players.at(i); }
	}
	return NULL;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
SliderCar** Game::GetPlayers()
{
	SliderCar** ids = new SliderCar*[_players.size()];
	for (unsigned int i=0; i<_players.size(); i++)
	{
		SliderCar* car = _players.at(i);
		ids[i]=car;
	}
	return ids;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool Game::AddPlayer(unsigned int pID)
{
	for (unsigned int i=0; i<_players.size(); i++)
	{
		if (_players.at(i)->GetpId()==pID) { return false; }
	}
	_players.push_back(new SliderCar(pID));
	return true;
}

//-----------------------------------------------------------------------------
// Update the game
//-----------------------------------------------------------------------------
void Game::Update(float timeDelta)
{
	if (IsPaused()) { return; } // Return if paused

	for (unsigned int i=0; i<_players.size(); i++)	/// Loop through each player
	{

		SliderCar* sliderCar=_players.at(i);

		/* Check where each car is currently and then set specific values for each type of game board cell */
		int cell = GetBoardCellAt(sliderCar->GetPosition()->_1,sliderCar->GetPosition()->_3)[0];
		if (cell==5) {						// Cell 5 boost the player up proportional to their x/z magnitude
			//SliderCar.SetFloor(5.0f);
			float mag = Magnitude(&POVector3(sliderCar->GetCenter()->_velocity._1,0.0f,sliderCar->GetCenter()->_velocity._3));
			sliderCar->GetCenter()->AddForce(POVector3(0.0f, mag*0.1, 0.0f));
		} else if (cell==2) {				// Cell 2 sets the floor higher so the player gets closer to the camera
			sliderCar->SetFloor(5.0f);
		} else if (cell==1 ||cell==2 ||cell==3 ||cell==4) {	// Cells here are default, floor is 2.0f, so the slidercar floats
			sliderCar->SetFloor(2.0f);
		} else { // Any other cell (e.g. 0) the car is not on anything and therefor falls down.
			sliderCar->SetFalling(true);
			sliderCar->SetFloor(-100.0f);
		}

		/* Check if the car is falling down and then reset to the start */
		if (sliderCar->GetPosition()->_2<=-100.0f)
		{
			sliderCar->SetFalling(false);
			sliderCar->SetPosition(10.0f,0.0f,5.0f);
			sliderCar->SetOrientation(0.0f);
			sliderCar->GetCenter()->SetVelocity(0.0f,0.0f,0.0f);
		}

		/* Check to see if players are colliding with other players, unfinished currently does nothing useful. */
		for (unsigned int j=0; j<_players.size(); j++)
		{
			if (_players.at(j)->GetpId()!=sliderCar->GetpId())
			{
				if (InterSectCheck(sliderCar->GetBoundingSphere(), _players.at(j)->GetBoundingSphere())) {
					/* Here we would do something like calculate the seperating velocity and resolve the collisions and so on
					this probably should be coded into a seperate physics part of the game.
					Currently players just get boosted upwards if they move over each other.
					*/
					float v1 = Magnitude(&sliderCar->GetCenter()->_velocity);
					float v2 = Magnitude(&_players.at(j)->GetCenter()->_velocity);
					if (v1>v2)
					{
						_players.at(j)->GetCenter()->AddForce(POVector3(0.0f, 0.001f, 0.0f));
					} else if (v2>v1)
					{
						sliderCar->GetCenter()->AddForce(POVector3(0.0f, 0.001f, 0.0f));
					}
				}
			}
		}

		/* Check for a collision between an object and the player */
		for (unsigned int j=0; j<_objects.size(); j++)
		{
			if (InterSectCheck(sliderCar->GetBoundingSphere(), _objects.at(j)->GetBoundingSphere())) {
				if (_objects.at(j)->IsSpawned())
				{
					_objects.at(j)->PickUp();
					switch(_objects.at(j)->GetType())
					{
					case GAMEOBJECT_TYPE_BOOST:
						sliderCar->Boost();				// The player went over a boost and should speed up
					break;
					case GAMEOBJECT_TYPE_SLOW:
						// This would slow the player down if they collected it
					break;
					case GAMEOBJECT_TYPE_ROCKET:
						sliderCar->SetWeapon(WEAPON_TYPE_LASER, 1); // Enable the player's laser (fires a teapot)
					break;
					case GAMEOBJECT_TYPE_PROJECTILE:
						/* Here would go the collision with a projectile method, currently just
						causes the player to boost of the board. If hit */
						sliderCar->Boost();						
					break;
					}
					//sliderCar->Boost();
				}
			}
		}

		/* Update the player */
		sliderCar->Update(timeDelta);
	}

	/* Update all objects */
	for (unsigned int i=0; i<_objects.size(); i++)
	{
		_objects.at(i)->Update(timeDelta);
	}
}

//-----------------------------------------------------------------------------
// Load the board up, this could be read from a file, but for simplicity it is not
//-----------------------------------------------------------------------------
void Game::LoadBoard() {
	int tmpboard[10][10][2] = {
		{{3,'l'},{2,'v'},{2,'v'},{2,'v'},{3,'d'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'}},
		{{1,'>'},{0,'>'},{0,'>'},{0,'>'},{4,'r'},{2,'v'},{3,'d'},{0,'>'},{0,'>'},{0,'>'}},
		{{5,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{1,'<'},{0,'>'},{0,'>'},{0,'>'}},
		{{1,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{1,'<'},{0,'>'},{0,'>'},{0,'>'}},
		{{3,'+'},{2,'^'},{4,'-'},{0,'>'},{0,'>'},{0,'>'},{1,'<'},{0,'>'},{0,'>'},{0,'>'}},
		{{0,'>'},{0,'>'},{1,'>'},{0,'>'},{0,'>'},{0,'>'},{1,'<'},{0,'>'},{0,'>'},{0,'>'}},
		{{0,'>'},{0,'>'},{3,'+'},{2,'^'},{4,'-'},{0,'>'},{1,'<'},{0,'>'},{0,'>'},{0,'>'}},
		{{0,'>'},{0,'>'},{0,'>'},{0,'>'},{3,'+'},{2,'^'},{3,'o'},{0,'>'},{0,'>'},{0,'>'}},
		{{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'}},
		{{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'},{0,'>'}}
	};

	for (int i=0; i<10; i++)
	{
		for (int j=0; j<10; j++)
		{
			for (int k=0; k<2; k++) {
				_board[i][j][k]=tmpboard[i][j][k];
				if (tmpboard[i][j][0]==1) {
					AddObject(&POVector3(((i*10.0f)+5.0f), 0.0f, ((j*10.0f)+5.0f)), GAMEOBJECT_TYPE_ROCKET);
				}
			}
		}
	}

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
int* Game::GetBoardCellAt(float x, float z)
{
	if (x <0.0f || z <0.0f)	{ return _board[10][10]; }
	int ix=(int)((x)/10.0f);
	int iz=(int)((z)/10.0f);
	return _board[ix][iz];
}

//-----------------------------------------------------------------------------
// Reset all players to the starting point
//-----------------------------------------------------------------------------
void Game::ResetPlayers()
{
	for (unsigned int i=0; i<_players.size(); i++)
	{
		_players.at(i)->SetPosition(10.0f,0.0f,5.0f);
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void Game::ResetPlayer(int pID)
{
//
}

//-----------------------------------------------------------------------------
// Does one sphere intersect another?
//-----------------------------------------------------------------------------
bool Game::InterSectCheck(POSphere r1, POSphere r2)
{
	float mag = SquareMagnitude(&(r1._pos-r2._pos));
	float min = r1._radius + r2._radius;
	return (mag<=(min*min));
}

//-----------------------------------------------------------------------------
// Return all objects in the game
//-----------------------------------------------------------------------------
GameObject** Game::GetObjects()
{
	GameObject** gos = new GameObject*[_objects.size()];
	for (unsigned int i=0; i<_objects.size(); i++)
	{
		GameObject* go = _objects.at(i);
		gos[i]=go;
	}
	return gos;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool Game::AddObject(POVector3* pos, POVector3* force, PointMass* target, GAMEOBJECT_TYPE type)
{
	GameObject* obj = new GameObject(pos, type, target);
	obj->GetCenter()->AddForce(*force);
	_objects.push_back(obj);

	return true;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool Game::AddObject(POVector3* pos, GAMEOBJECT_TYPE type)
{
	_objects.push_back(new GameObject(pos, type));
	return true;
}