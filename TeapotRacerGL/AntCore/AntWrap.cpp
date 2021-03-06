#include "AntWrap.h"
#include "GLRenderer.h"

PRENDERCALLBACK RenderCallback;
PMSGCALLBACK MsgCallback;

//-----------------------------------------------------------------------------
// Init Window, and also the selected device
//-----------------------------------------------------------------------------
bool AntWrapInit(AntRenderer** device, AntSettings settings, wchar_t* wndName)
{
	/* Choose specific Init - DirectX or OpenGL */
	if (settings._deviceType==ANTDEVICE_DX) // DirectX
	{
		fprintf(stderr,"DirectXNotImplementedError(CannotContinue)");
		return false;
		//MessageBox(0, L"DirectXNotImplementedError(CannotContinue)", 0, 0);
	} else if (settings._deviceType==ANTDEVICE_GL) { // Open GL
		*device = new GLRenderer();
		(*device)->Init(settings);
	//} else if (settings._deviceType==ANTDEVICE_GLFW) // Open GL
	//{
	//	fprintf(stderr,"GLFWNotImplementedError(CannotContinue)");
		//MessageBox(0, L"GLFWNotImplementedError(CannotContinue)", 0, 0);
	} else
	{
		fprintf(stderr,"InvalidRendererChoice(CannotContinue)");
		//MessageBox(0, L"InvalidRendererChoice(CannotContinue)", 0, 0);
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// Cleanup
//-----------------------------------------------------------------------------
bool AntWrapQuit(AntRenderer* renderer)
{
	renderer->Cleanup();
	delete renderer;
	return true;
}

//-----------------------------------------------------------------------------
// Our main loop
//-----------------------------------------------------------------------------
int AntWrapRun()
{////////// MOVE OUT SDL EVENTS
	SDL_Event event;
	float lastTime = SDL_GetTicks();//(float)timeGetTime();
	ANTGUI_EVENT msg;
	UINT p1 = 0;
	UINT p2 = 0;
	while(true)
	{
		if(SDL_PollEvent(&event))
		{
			//if (event.type == SDL_QUIT) return 1;
			//translate SDL message to ant message format
		  	  bool down = false;
			  switch(event.type)
			  {
			  	  case SDL_QUIT: return 1; break;
			  	  case SDL_MOUSEMOTION:
			  		  msg = ANTGUI_EVENT_MOUSEMOVE;
			  		  p2 = SETDWORD(event.motion.x, event.motion.y);
			  		  break;
			  	  case SDL_KEYDOWN:
			  		  down = true;
			  	  case SDL_KEYUP:
			  		  msg = (down) ? ANTGUI_EVENT_KEYDOWN : ANTGUI_EVENT_KEYUP;
			  		  switch(event.key.keysym.sym)
			  		  {
			  		  	  case SDLK_LEFT: p1 = ANTGUI_EVENT_KEY_LEFT; break;
			  		  	  case SDLK_UP:  p1 = ANTGUI_EVENT_KEY_UP; break;
			  		  	  case SDLK_DOWN:  p1 = ANTGUI_EVENT_KEY_DOWN; break;
			  		  	  case SDLK_RIGHT:  p1 = ANTGUI_EVENT_KEY_RIGHT; break;
			  		  	  case SDLK_BACKSPACE:  p1 = ANTGUI_EVENT_KEY_BACK; break;
			  		  	  case SDLK_ESCAPE:  p1 = ANTGUI_EVENT_KEY_ESCAPE; break;
			  		  	  case SDLK_RETURN:  p1 = ANTGUI_EVENT_KEY_RETURN; break;
			  		  	  case SDLK_SPACE:  p1 = ANTGUI_EVENT_KEY_SPACE; break;
			  		  	  case SDLK_LSHIFT:  p1 = ANTGUI_EVENT_KEY_SHIFT; break;
			  		  	  default: break;
			  		  }
			  		  //p1 = event.key.keysym.scancode;
			  	  break;
			  	  case SDL_MOUSEBUTTONDOWN:
			  		  down = true;
			  		  fprintf(stderr, "Mouse click at %d, %d\n",event.button.x, event.button.y);
			  	  case SDL_MOUSEBUTTONUP:
			  		  switch (event.button.button)
			  		  {
			  		  	  case SDL_BUTTON_LEFT: msg = (down) ? ANTGUI_EVENT_LBUTTONDOWN : ANTGUI_EVENT_LBUTTONUP; break;
			  		  	  case SDL_BUTTON_MIDDLE: msg = (down) ? ANTGUI_EVENT_MBUTTONDOWN : ANTGUI_EVENT_MBUTTONUP; break;
			  		  	  case SDL_BUTTON_RIGHT: msg = (down) ? ANTGUI_EVENT_RBUTTONDOWN : ANTGUI_EVENT_RBUTTONUP; break;
			  		  	  default: break;
			  		  }
			  		  p2 = SETDWORD(event.button.y, event.button.x);
			  		  break;
			  }
			if (MsgCallback!=NULL) { MsgCallback(msg, p1, p2); }
		}
					{
						float time = SDL_GetTicks();//(float)timeGetTime();
						float timeDelta = time - lastTime;

						/* If personal callbacks are set then call */
						if (RenderCallback!=NULL) { RenderCallback(timeDelta); }
						lastTime=time;

					}
	}
	return 0;
}


//-----------------------------------------------------------------------------
// Set our personal callbacks so we can pass to them and do our own processing
//-----------------------------------------------------------------------------
void AntWrapSetRenderCallBack(PRENDERCALLBACK callback) {
	RenderCallback=callback;
}

void AntWrapSetMsgCallBack(PMSGCALLBACK callback) {
	MsgCallback=callback;
}
