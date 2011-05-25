#include "AntWrap.h"
#include "DXRenderer.h"

PRENDERCALLBACK RenderCallback;
PMSGCALLBACK MsgCallback;

//-----------------------------------------------------------------------------
// Init Window, and also the selected device
//-----------------------------------------------------------------------------
bool AntWrapInit(AntRenderer** device, AntSettings settings, HINSTANCE hInstance, LPCWSTR wndName)
{
	/* Window creation */
	//HRESULT hr;

	WNDCLASS wc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
	wc.hCursor			= NULL;
	wc.hIcon			= NULL;
	wc.hInstance		= hInstance;
	wc.lpfnWndProc		= WndProc;
	wc.lpszClassName	= wndName;
	wc.lpszMenuName		= NULL;
	wc.style			= CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&wc)) { MessageBox(0, L"RegisterClassError(CannotContinue)", 0, 0); return false; }

	HWND hWnd = 0;
	hWnd = CreateWindow(
		wndName,
		wndName,
		WS_TILED,
		0,
		0,
		settings._width,
		settings._height,
		0,
		0,
		hInstance,
		0
	);

	if (!hWnd) { MessageBox(0, L"CreateWindowError(CannotContinue)", 0, 0); return false; }

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	/* Choose specific Init - DirectX or OpenGL */
	if (settings._deviceType==ANTDEVICE_DX) // DirectX
	{
		*device = new DXRenderer();
		(*device)->Init(hWnd, settings);

	} else if (settings._deviceType==ANTDEVICE_GL) { // Open GL
		MessageBox(0, L"OpenGLNotImplementedError(CannotContinue)", 0, 0);
		return false;
	} else
	{
		MessageBox(0, L"InvalidRendererChoice(CannotContinue)", 0, 0);
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
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	float lastTime = (float)timeGetTime();

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else
		{
			float time = (float)timeGetTime();
			float timeDelta = time - lastTime;

			/* If personal callbacks are set then call */
			if (RenderCallback!=NULL) { RenderCallback(timeDelta); }
			lastTime=time;

		}
	}
	return msg.wParam;
}

//-----------------------------------------------------------------------------
// WndProc (event) callback)
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		/* If personal callbacks are set then pass the message onwards */
		if (MsgCallback!=NULL) { MsgCallback(msg, wParam, lParam); }
		break;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
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