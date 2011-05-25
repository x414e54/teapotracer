#include "GLRenderer.h"

//-----------------------------------------------------------------------------
// Init opengl
//-----------------------------------------------------------------------------
bool GLRenderer::Init(void* wnd, AntSettings settings)
{
		/*HRESULT hr;

		IDirect3D9* d3d9;
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

		D3DCAPS9 caps;
		d3d9->GetDeviceCaps(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			&caps);
		
		D3DPRESENT_PARAMETERS d3dpp;
		d3dpp.BackBufferWidth				= settings._width;
		d3dpp.BackBufferHeight				= settings._height;
		d3dpp.BackBufferFormat				= D3DFMT_A8R8G8B8;
		d3dpp.BackBufferCount				= 1;
		d3dpp.MultiSampleType				= (D3DMULTISAMPLE_TYPE)settings._multiSampleType;
		d3dpp.MultiSampleQuality			= 0;
		d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow					= 0;
		d3dpp.Windowed						= !settings._fullscreen;
		d3dpp.EnableAutoDepthStencil		= true;
		d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;
		d3dpp.Flags							= 0;
		d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
		d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;

		_device = 0;
		hr = d3d9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				(IDirect3DDevice9 **)&_device);

		d3d9->Release();

		if (FAILED(hr))	{ MessageBox(0, L"DXInitFailedError(CannotContinue)", 0, 0); return false; }

		D3DXCreateSprite(_device, &_sprite);*/

		return true;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::BeginScene()
{
	if (_device)
	{
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::EndScene()
{
	if (_device)
	{
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::SetView(POVector3* pos, POVector3* target)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::SetProjection()
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawMesh(UINT meshId, POVector3* pos, POVector3* o)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawSprite(UINT textureId, RECT* src, POVector3* pos, float sx, float sw, float rotation)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawText(UINT fontId, std::wstring string, RECT* dst, AntFontColorARGB* fontColor)
{
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::DrawQuad(UINT textureID, RECT* rect, POVector3* pos, POVector3* o)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddFont(std::wstring typeFace, UINT width, UINT height)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddTexture(std::wstring fileName)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void GLRenderer::AddMesh(std::wstring fileName)
{

}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool GLRenderer::Cleanup()
{

	return false;
}
