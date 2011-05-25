#include "AntWrap.h"
#include "GLRenderer.h"

PRENDERCALLBACK RenderCallback;
PMSGCALLBACK MsgCallback;

//-----------------------------------------------------------------------------
// Init Window, and also the selected device
//-----------------------------------------------------------------------------
bool AntWrapInit(AntRenderer** device, AntSettings settings, wchar_t* wndName)
{
	// This could be moved to antwrapsdl and have two versions?

	if(SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		throw SDL_GetError();
	}

	SDL_Surface *bg;
	SDL_Surface* screen = SDL_SetVideoMode(640, 480, 0, 0);
	SDL_WM_SetCaption("NAme","O");

	/* Choose specific Init - DirectX or OpenGL */
	if (settings._deviceType==ANTDEVICE_DX) // DirectX
	{
		//MessageBox(0, L"DirectXNotImplementedError(CannotContinue)", 0, 0);

	} else if (settings._deviceType==ANTDEVICE_GL) { // Open GL
		*device = new GLRenderer();
		(*device)->Init(screen, settings);
		return false;
	} else
	{
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
{
	SDL_Event event;
	float lastTime = 0;//(float)timeGetTime();
	ANTGUI_EVENT msg;
	while(true)
	{
		if(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) return 1;
			//translate SDL message to ant message format
			if (MsgCallback!=NULL) { MsgCallback(msg, 0, 0); }
		}
					{
						float time = 0;//(float)timeGetTime();
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
