//*****************************************************************************
//-----------------------------------------------------------------------------
// TeapotRacerGL main class
//
//
// This is the main class to show the creation of a game with the AntCore engine
// code. AntCore engine is unfinished but serves as a demo and sample code.
//-----------------------------------------------------------------------------
//*****************************************************************************


//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

// Other includes
//#include <windows.h>

// Ant Core Includes
#include "AntCore/AntSettings.h"
#include "AntCore/AntWrap.h"
#include "AntCore/AntGUI.h"
#include "AntCore/AntActor.h"
#include "AntCore/AntCamera.h"
#include "AntCore/AntMath.h"
#include "AntCore/GLRenderer.h"

// Game Specific includes
#include "Game.h"
#include "CarAI.h"
#include "PlayerControls.h"
#include "SliderCar.h"

//-----------------------------------------------------------------------------
// Globals
//-----------------------------------------------------------------------------
AntGUIDialog			StartDlg;
AntGUIDialog			MenuDlg;
AntGUIDialog			HudDlg;
AntCameraFollower		Camera;
AntRenderer*			Renderer;
AntSettings				Settings;

Game					Game;

PlayerControls*			Controls;
CarAI*					AI;

float rotation;
bool TargetingAI=false;							// Bool to show if the camera is targeting AI or PLAYERONE											
#define PLAYERONE_ID			0
#define AIPLAYER_ID				1

// Texture Ids.
#define TID_GUI 0
#define TID_TRACKSTRAIGHT 1
#define TID_TRACKBRIDGE 2
#define TID_TRACKTURN 3
#define TID_BACKGROUND 4
#define TID_PLAYERBLIP 5

//-----------------------------------------------------------------------------
// Forward declarations
//-----------------------------------------------------------------------------
void Render(float timeDelta);
bool Event(UINT msg, UINT p1, UINT p2);
void Init();
void RunSettings();
void QuitEventHandler(void *vParam);
void ResumeEventHandler(void *vParam);
void SettingsEventHandler(void *vParam);
void NewGameEventHandler(void *vParam);

//-----------------------------------------------------------------------------
// Main 
//-----------------------------------------------------------------------------
int main(int argc, char* argv)
{
	//WIN32_FIND_DATA findData;
	//HANDLE hSearch;

	//hSearch = FindFirstFile(L"settings.dat", &findData);

	//if (hSearch == INVALID_HANDLE_VALUE) { RunSettings(); }
	
	//FindClose(hSearch);

	Settings.Read();
	rotation=0.0f;
	if(!AntWrapInit(&Renderer, Settings, L"TeapotRacer")) {
		fprintf(stderr,"DisplayInitFailed(CannotContinue)"); return -1;
		//MessageBox(0, L"DisplayInitFailed(CannotContinue)", 0, 0);
	}

	AntWrapSetRenderCallBack(Render);
	AntWrapSetMsgCallBack(Event);
	Init();
	AntWrapRun();
	AntWrapQuit(Renderer);

	delete Controls;
	delete AI;

	return 0;
}

//-----------------------------------------------------------------------------
// Init everything application specific here
//-----------------------------------------------------------------------------
void Init()
{
	StartDlg.Init(Renderer);

	/// MOVE TO CSS

	/* The game into menu Dialog */
	StartDlg.Init(Renderer);
	StartDlg.AddButton(0,L"New",100,150,18,63);
	StartDlg.GetButton(0)->SetEventCaller(NewGameEventHandler);
	StartDlg.AddButton(1,L"Load",100,190,18,63);
	StartDlg.AddButton(2,L"Settings",100,230,18,63);
	StartDlg.GetButton(2)->SetEventCaller(SettingsEventHandler);
	StartDlg.AddButton(3,L"Quit",100,270,18,63);
	StartDlg.GetButton(3)->SetEventCaller(QuitEventHandler);
	StartDlg.Show(true);

	/* The Escape Menu Dialog */
	MenuDlg.Init(Renderer);
	MenuDlg.AddButton(0,L"Resume",100,150,18,63);
	MenuDlg.GetButton(0)->SetEventCaller(ResumeEventHandler);
	MenuDlg.AddButton(1,L"Save",100,190,18,63);
	MenuDlg.AddButton(2,L"Load",100,230,18,63);
	MenuDlg.AddButton(3,L"Quit",100,270,18,63);
	MenuDlg.GetButton(3)->SetEventCaller(QuitEventHandler);

	/* The Hud Dialog */
	HudDlg.Init(Renderer);
	HudDlg.AddImageText(0,L"Resume",100,150,18,63);
	HudDlg.AddImageText(1,L"Save",100,190,18,63);
	HudDlg.Show(true);

	/* Add two new Players to the game */
	Game.AddPlayer(PLAYERONE_ID);
	Game.AddPlayer(AIPLAYER_ID);

	/* We want the camera to always look at PLAYERONE */
	Camera.SetTarget(Game.GetPlayer(PLAYERONE_ID)->GetCenter());

	/* Send the game's pointer to the AI and Controls so they can access information such as the board. */
	Controls = new PlayerControls(&Game);
	AI = new CarAI(&Game);

	/* We want the camera to always look at PLAYERONE */
	Controls->SetActor(Game.GetPlayer(0));

	/* We want the camera to always look at PLAYERONE */
	AI->SetActor(Game.GetPlayer(AIPLAYER_ID));
	Game.ResetPlayers();

	/* Call the SetProjection method in the Renderer, which should set up the Projection matrix, this should be extended to take aguments later-on. */
	Renderer->SetProjection();

	/* AddMeshes, and textures - in this case AddMesh will always create the DirectX teapot prefab mesh. */
	Renderer->AddMesh(L"blah");
	Renderer->AddTexture(TID_GUI, L"firsttexture.bmp");
	Renderer->AddTexture(TID_TRACKSTRAIGHT, L"trackstraight.bmp");
	Renderer->AddTexture(TID_TRACKBRIDGE, L"trackbridge.bmp");
	Renderer->AddTexture(TID_TRACKTURN, L"trackturn.bmp");
	Renderer->AddTexture(TID_BACKGROUND, L"background.bmp");
	Renderer->AddTexture(TID_PLAYERBLIP, L"playerblip.bmp");
}

//-----------------------------------------------------------------------------
// Our Renderer method/callback - here we do rendering and any other processing
//-----------------------------------------------------------------------------
void Render(float timeDelta)
{
	Renderer->BeginScene(); // Being scene, this is an abstraction of the DirectX BeginScene method, and alot of other renerers also need the scene to be started in some format.

	if (Game.GetState()==ANTGAME_STATE_MENU)															// We are in the start menu state
	{
		rotation+=timeDelta;
		Renderer->SetView(POVector3(sin(rotation/100.0f),10.0f,-3.0f),POVector3(0.0f,0.0f,0.0f));		// Set view matrix
		StartDlg.Render(timeDelta);																		// Render Start GUI
		Renderer->DrawMesh(0,POVector3(0.0f, 0.0f, 0.0f), POVector3(0.0f, 0.0f, 0.0f));

	} else if (Game.GetState()==ANTGAME_STATE_PLAYING)													// The game is playing so we now render the scene
	{
		/* Send updates*/
		AI->Update(timeDelta);
		Controls->Update(timeDelta);
		Game.Update(timeDelta);
		Camera.Update(timeDelta);

		/* Set the text for the speed and throttle setting on the HUD */
		HudDlg.GetImageText(0)->SetText(Game.GetPlayer(PLAYERONE_ID)->GetSpeedString());
		HudDlg.GetImageText(1)->SetText(Game.GetPlayer(PLAYERONE_ID)->GetThrottleString());

		/* Set the view matrix, the camera posion and target are specifically designed for this */
		Renderer->SetView(Camera.GetPosition(),Camera.GetTarget());

		/* Set the background */
		RECT rect;
		SetRect(&rect,0,0,1000,1000);
		Renderer->DrawQuad(TID_BACKGROUND,&rect,POVector3(Camera.GetTarget()->_1-500,-100.0f,Camera.GetTarget()->_3-500),POVector3(0.0f,0.0f,0.0f));


		/* Draw the game board and the mini game board */
		SetRect(&rect,-5,-5,5,5);
		float xoffset=200.0f;
		float quadxoffset=5.0f;
		float quadyoffset=5.0f;
		float yoffset=300.0f;
		for (int i=0; i<10; i++)
		{
			for (int j=0; j<10; j++)
			{
				switch(Game.GetBoardCell(i,j)[0])
				{
				case 1:
					Renderer->DrawQuad(TID_TRACKSTRAIGHT,&rect,POVector3((10.0f*i)+quadxoffset,0.0f,(10.0f*j+quadyoffset)),POVector3(0.0f,PI/2,0.0f)); // Different pieces will need different rotations
					Renderer->DrawSprite(TID_TRACKSTRAIGHT,NULL,POVector3((16.0f*i)+xoffset,(-16.0f*j)+yoffset,0.0f), 0.125f, 0.125f, 0.0f);
				break;
				case 2:
					Renderer->DrawQuad(TID_TRACKSTRAIGHT,&rect,POVector3((10.0f*i)+quadxoffset,0.0f,(10.0f*j+quadyoffset)),POVector3(0.0f,0.0f,0.0f));
					Renderer->DrawSprite(TID_TRACKSTRAIGHT,NULL,POVector3((16.0f*i)+xoffset,(-16.0f*j)+yoffset,0.0f), 0.125f, 0.125f, 0.0f);
				break;
				case 3:
					Renderer->DrawQuad(TID_TRACKTURN,&rect,POVector3((10.0f*i)+quadxoffset,0.0f,(10.0f*j+quadyoffset)),POVector3(0.0f,PI/2,0.0f));
					Renderer->DrawSprite(TID_TRACKTURN,NULL,POVector3((16.0f*i)+xoffset,(-16.0f*j)+yoffset,0.0f), 0.125f, 0.125f, 0.0f);
				break;
				case 4:
					Renderer->DrawQuad(TID_TRACKTURN,&rect,POVector3((10.0f*i)+quadxoffset,0.0f,(10.0f*j+quadyoffset)),POVector3(0.0f,0.0f,0.0f));
					Renderer->DrawSprite(TID_TRACKTURN,NULL,POVector3((16.0f*i)+xoffset,(-16.0f*j)+yoffset,0.0f), 0.125f, 0.125f, 0.0f);
				break;
				case 5:
					Renderer->DrawQuad(TID_TRACKBRIDGE,&rect,POVector3((10.0f*i)+quadxoffset,0.0f,(10.0f*j+quadyoffset)),POVector3(0.0f,0.0f,0.0f));
					Renderer->DrawSprite(TID_TRACKBRIDGE,NULL,POVector3((16.0f*i)+xoffset,(-16.0f*j)+yoffset,0.0f), 0.125f, 0.125f, 0.0f);
				break;

				}
			}
		}

		/* Get players and render their meshes and also a small sprite on the mini gameboard to show their positions */
		SliderCar** players = Game.GetPlayers();
		int noPlayers = Game.GetNumberOfPlayers();
		for (SliderCar** i=players; i<players+noPlayers; i++)
		{
			Renderer->DrawMesh(0,(*i)->GetPosition(), POVector3(0.0f, (*i)->GetOrientation(), 0.0f));
			Renderer->DrawSprite(TID_PLAYERBLIP,NULL,POVector3(((*i)->GetPosition()->_1*1.5)+xoffset,(-(*i)->GetPosition()->_3*1.5)+yoffset,0.0f), 1.0f, 1.0f, 0.0f);
		}			
		delete players;

		/* Get any game objects render them on in the correct posision on the board*/
		GameObject** objects = Game.GetObjects();
		int noobjects = Game.GetNumberOfObjects();
		for (GameObject** i=objects; i<objects+noobjects; i++)
		{
			if ((*i)->IsSpawned()) {
				Renderer->DrawMesh(0,(*i)->GetPosition(), POVector3(0.0f, (*i)->GetOrientation(), 0.0f));
			}
		}			
		delete objects;

		/* Render any gui components last so they show up on top */
		HudDlg.Render(timeDelta);
		MenuDlg.Render(timeDelta);

	} else if (Game.GetState()==ANTGAME_STATE_CREDITS)					// Credits not included.
	{

	}

	Renderer->EndScene(); // End scene, this is an abstraction of the DirectX EndScene method,
}

//-----------------------------------------------------------------------------
// Our event callback
//-----------------------------------------------------------------------------
bool Event(UINT msg, UINT p1, UINT p2)
{
	if (Game.GetState()==ANTGAME_STATE_MENU)						// Start Menu state, we just need to allow quit and send everythign else off to the GUI
	{
		switch(msg)
		{
		case ANTGUI_EVENT_QUIT:
			//PostQuitMessage(0);
		break;
		case ANTGUI_EVENT_KEYDOWN:
			switch(p1)
			{
			case ANTGUI_EVENT_KEY_ESCAPE:
				//PostQuitMessage(0);
			break;
			}
		break;
		}

		StartDlg.Event(msg, p1, p2);

	} else if (Game.GetState()==ANTGAME_STATE_PLAYING)				// Game is playing, allow quit and show menu dlg on escape, return swaps the camera between players
	{																// we must then send all other events to the GUI's
		switch(msg)
		{
		case ANTGUI_EVENT_QUIT:
			//PostQuitMessage(0);
		break;
		case ANTGUI_EVENT_KEYDOWN:
			switch(p1)
			{
			case ANTGUI_EVENT_KEY_ESCAPE:
				MenuDlg.Show(true);
			break;
			case ANTGUI_EVENT_KEY_RETURN:
				if (!TargetingAI)
				{
					TargetingAI=true;
					Camera.SetTarget(Game.GetPlayer(AIPLAYER_ID)->GetCenter());
				} else
				{
					TargetingAI=false;
					Camera.SetTarget(Game.GetPlayer(PLAYERONE_ID)->GetCenter());
				}
			break;
			}
		break;
		}

		MenuDlg.Event(msg, p1, p2);
		Controls->Event(msg, p1, p2);

	} else if (Game.GetState()==ANTGAME_STATE_CREDITS)			// State has been included but does nothing as of yet.
	{

	}
	return false;
}

//-----------------------------------------------------------------------------
// On calling will open up the Settings.exe mfc
//-----------------------------------------------------------------------------
void RunSettings()
{
	/*STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;

	ZeroMemory(&sInfo, sizeof(STARTUPINFO));
	sInfo.cb = sizeof(STARTUPINFO);
	ZeroMemory(&pInfo, sizeof(PROCESS_INFORMATION));

	if(!CreateProcess(L".\\Settings.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo)) { MessageBox(0, L"CreateProcessError(Continuing)", 0, 0); return; }

	WaitForSingleObject(pInfo.hProcess, INFINITE);

	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);*/
}

//-----------------------------------------------------------------------------
// Collection of event handlers for our GUI
//-----------------------------------------------------------------------------
void QuitEventHandler(void* vParam)			// On pressing the quit button
{
	//PostQuitMessage(0);
	exit(0);
}

void ResumeEventHandler(void* vParam)	// On pressing resume
{
	MenuDlg.Show(false);
}

void SettingsEventHandler(void* vParam)		// On pressing settings
{
	//PostQuitMessage(0);

	RunSettings();
}

void NewGameEventHandler(void* vParam)		// On pressing new game
{
	Game.SetState(ANTGAME_STATE_PLAYING);
}
