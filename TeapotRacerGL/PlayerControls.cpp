#include "PlayerControls.h"

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
PlayerControls::PlayerControls(Game* game) {
	_game=game;
	_actor=NULL;
	_forward=false;
	_back=false;
	_left=false;
	_right=false;
	_break=false;
}

//-----------------------------------------------------------------------------
// Event method for handling keyboard input
//-----------------------------------------------------------------------------
bool PlayerControls::Event(UINT msg, UINT p1, UINT p2)
{
	if (_actor==NULL) { return false; }
	switch(msg)
	{
	case ANTGUI_EVENT_KEYDOWN:
		switch(p1)
		{
		case ANTGUI_EVENT_KEY_LEFT:
			_left=true;
		break;
		case ANTGUI_EVENT_KEY_RIGHT:
			_right=true;
		break;
		case ANTGUI_EVENT_KEY_UP:
			_forward=true;
			_actor->SetGear(SLIDERCAR_GEAR_1);
		break;
		case ANTGUI_EVENT_KEY_DOWN:
			_back=true;
			_actor->SetGear(SLIDERCAR_GEAR_REVERSE);
		break;
		case ANTGUI_EVENT_KEY_SPACE:
			_break=true;
			_actor->SetBreak(true);
		break;
		case ANTGUI_EVENT_KEY_SHIFT:
			_actor->Boost();
		break;
		case 'E':
			if (_actor->Fire()) 
			{
				POVector3 pos;
				POVector3 force;
				float x=cos(_actor->GetOrientation())*0.01f;
				float z=sin(_actor->GetOrientation())*-0.01f;
				force._1=x;
				force._2=0.0f;
				force._3=z;
				pos._1=_actor->GetPosition()->_1+(250.0f*x);
				pos._2=_actor->GetPosition()->_2;
				pos._3=_actor->GetPosition()->_3+(250.0f*z);
				_game->AddObject(&pos, &force, NULL, GAMEOBJECT_TYPE_PROJECTILE);
			}
		break;
		}
	break;
	case ANTGUI_EVENT_KEYUP:
		switch(p1)
		{
		case ANTGUI_EVENT_KEY_LEFT:
			_left=false;
			_actor->SetRotation(0.0f);
		break;
		case ANTGUI_EVENT_KEY_RIGHT:
			_right=false;
			_actor->SetRotation(0.0f);
		break;
		case ANTGUI_EVENT_KEY_UP:
			_forward=false;
		break;
		case ANTGUI_EVENT_KEY_DOWN:
			_back=false;
		break;
		case ANTGUI_EVENT_KEY_SPACE:
			_break=false;
			_actor->SetBreak(false);
		break;
		}
	break;
	}
	return false;
}

void PlayerControls::Update(float timeDelta)
{
	if (_forward || _back)	{_actor->SetThrottle(_actor->GetThrottle()+timeDelta/700.0f);}										else { _actor->SetThrottle(_actor->GetThrottle()-timeDelta/700.0f); }
	if (_left)		{ _actor->SetRotation(_actor->GetRotation()-timeDelta/100000.0f);}											else { }
	if (_right)		{ _actor->SetRotation(_actor->GetRotation()+timeDelta/100000.0f);}											else { }
	//if (_break)		{_actor->SetBreak(true); }																		else { }
}
