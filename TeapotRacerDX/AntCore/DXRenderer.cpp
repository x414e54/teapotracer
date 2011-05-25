#include "DXRenderer.h"

//-----------------------------------------------------------------------------
// Init directx
//-----------------------------------------------------------------------------
bool DXRenderer::Init(HWND hWnd, AntSettings settings)
{
		HRESULT hr;

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

		D3DXCreateSprite(_device, &_sprite);

		return true;
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::BeginScene()
{
	if (_device)
	{
		_device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

		_device->BeginScene();

	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::EndScene()
{
	if (_device)
	{
			_device->EndScene();
			_device->Present(0,0,0,0);
			// We will need to deal with, Device lost and reset errors, however this is not implemented yet
	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::SetView(POVector3* pos, POVector3* target)
{
    D3DXMATRIX mView;
	D3DXMatrixLookAtLH(&mView, &D3DXVECTOR3 (pos->_1, pos->_2, pos->_3), &D3DXVECTOR3(target->_1, target->_2, target->_3), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	_device->SetTransform(D3DTS_VIEW, &mView);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::SetProjection()
{
	D3DXMATRIX mProj;
	D3DXMatrixPerspectiveFovLH(&mProj,
			D3DX_PI * 0.5f,
			(float)800 / (float)600,
			1.0f,
			1000.0f);
	_device->SetTransform(D3DTS_PROJECTION, &mProj);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::DrawMesh(UINT meshId, POVector3* pos, POVector3* or)
{
	// Very Inefficient should be moved to one unified function called before rendering of all m
    /*_device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    _device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    _device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    _device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    _device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
    _device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    _device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED );
    _device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    _device->SetRenderState( D3DRS_FOGENABLE, FALSE );
    _device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );*/
	_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    //_device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

	D3DXMATRIX mRX,mRY,mRZ,mTransl;

	D3DXMatrixRotationX(&mRX, or->_1);        // Pitch
	D3DXMatrixRotationY(&mRY, or->_2);        // Yaw
	D3DXMatrixRotationZ(&mRZ, or->_3);        // Roll

	D3DXMatrixTranslation(&mTransl, pos->_1, pos->_2, pos->_3);

	D3DXMATRIX mWorld=(mRX*mRY*mRZ)*mTransl;

	_device->SetTransform(D3DTS_WORLD, &mWorld);

	static_cast<ID3DXMesh*>(GetMesh(meshId)->mesh)->DrawSubset(0);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::DrawSprite(UINT textureId, RECT* src, POVector3* pos, float sx, float sw, float rotation)
{
	// Very Inefficient should be moved to one unified function called before rendering of all sprites
	/*
    _device->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    _device->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
    _device->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
    _device->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
    _device->SetRenderState( D3DRS_SEPARATEALPHABLENDENABLE, FALSE );
    _device->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD );
    _device->SetRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED );
    _device->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    _device->SetRenderState( D3DRS_FOGENABLE, FALSE );
    _device->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
    _device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
    _device->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	*/
	_sprite->Begin(D3DXSPRITE_ALPHABLEND);
	AntTexture* antTexture = GetTexture(textureId);
	IDirect3DTexture9* texture = static_cast<IDirect3DTexture9*>(antTexture->texture);
	D3DXMATRIX mTexture;
	D3DXVECTOR2 scaling(sx,sw);
	D3DXMatrixTransformation2D(&mTexture,NULL,0.0f,&scaling,&D3DXVECTOR2(antTexture->w/2,antTexture->h/2),rotation,&D3DXVECTOR2(pos->_1,pos->_2));
	_sprite->SetTransform(&mTexture);
	_sprite->Draw(texture, src, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f) , 0xFFFFFFFF);
	_sprite->End();
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::DrawText(UINT fontId, LPCWSTR string, RECT* dst, AntFontColorARGB* fontColor)
{
	// Again inefficient as the font is drawn to no sprite and DirectX has to create one every time we call DrawText
	static_cast<ID3DXFont*>(GetFont(fontId)->font)->DrawText(NULL, string, -1, dst, DT_CENTER, D3DCOLOR_ARGB(fontColor->_A, fontColor->_R, fontColor->_G, fontColor->_B));
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::DrawQuad(UINT textureID, RECT* rect, POVector3* pos, POVector3* or)
{
	D3DXMATRIX mRX,mRY,mRZ,mTransl;

	D3DXMatrixRotationX(&mRX, or->_1);        // Pitch
	D3DXMatrixRotationY(&mRY, or->_2);        // Yaw
	D3DXMatrixRotationZ(&mRZ, or->_3);        // Roll

	D3DXMatrixTranslation(&mTransl, pos->_1, pos->_2, pos->_3);

	D3DXMATRIX mWorld=(mRX*mRY*mRZ)*mTransl;

    VERTEX vertices[4] =
            {
				(float)rect->left,  0.0f,		(float)rect->bottom, 0.0f, 1.0f,
                (float)rect->right, 0.0f,		(float)rect->bottom, 1.0f, 1.0f,
                (float)rect->right, 0.0f,	(float)rect->top, 1.0f, 0.0f,
                (float)rect->left,  0.0f,	(float)rect->top, 0.0f, 0.0f,
            };

	//_device->SetVertexShader( NULL );
	//_device->SetPixelShader( NULL );

	_device->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	_device->SetRenderState(D3DRS_LIGHTING, FALSE);
	_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	_device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	_device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//_device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
	//_device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	//_device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);

    _device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
	HRESULT hr = _device->SetTexture(0,static_cast<IDirect3DBaseTexture9 *>(GetTexture(textureID)->texture));
	_device->SetTransform(D3DTS_WORLD, &mWorld);
	_device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, vertices, sizeof(VERTEX));
		//
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::AddFont(LPCWSTR typeFace, LONG width, LONG height)
{
	HRESULT hr;
	AntFont* font = new AntFont;

    ZeroMemory(font, sizeof(AntFont));

	StringCchCopy(font->typeFace,MAX_PATH,typeFace);
	font->w=width;
	font->h=height;

	if (_device!=NULL) {
		hr = D3DXCreateFont(_device,
				font->h,
				font->w,
				0,
				1,
				FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,
				font->typeFace,
				(LPD3DXFONT *)&font->font);
	}


	_fonts.push_back(font);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::AddTexture(LPCWSTR fileName)
{
	AntTexture* texture =  new AntTexture;
	texture->h=0;
	texture->w=0;

	StringCchCopy(texture->fileName,MAX_PATH,fileName);

	HRESULT hr;
	if (_device!=NULL) {
		D3DXIMAGE_INFO info;
        hr = D3DXCreateTextureFromFileEx(_device,
			texture->fileName,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
            1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
            D3DX_DEFAULT,
			D3DX_DEFAULT,
			D3DCOLOR_ARGB(255,255,0,255),
			&info,
			NULL,
			(LPDIRECT3DTEXTURE9 *)&texture->texture);
		texture->h=info.Height;
		texture->w=info.Width;
	}

	if (hr==S_OK) {
		_textures.push_back(texture);
	} else {
		MessageBox(0, L"TextureLoadFailed(TryingToContinue)", 0, 0);

	}
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
void DXRenderer::AddMesh(LPCWSTR fileName)
{
	HRESULT hr;
	AntMesh* mesh = new AntMesh;

    ZeroMemory(mesh, sizeof(AntMesh));

	StringCchCopy(mesh->fileName,MAX_PATH,fileName);

	hr = D3DXCreateTeapot(_device, (LPD3DXMESH *)&mesh->mesh, NULL);

	_meshes.push_back(mesh);
}

//-----------------------------------------------------------------------------
// 
//-----------------------------------------------------------------------------
bool DXRenderer::Cleanup()
{
	for (unsigned int i=0; i<_textures.size(); i++)
	{
		static_cast<IDirect3DTexture9*>(_textures.at(i)->texture)->Release();
		delete _textures.at(i);
		_textures.at(i)=NULL;
	}
	_textures.clear();

	for (unsigned int i=0; i<_fonts.size(); i++)
	{
		static_cast<ID3DXFont*>(_fonts.at(i)->font)->Release();
		delete _fonts.at(i);
		_fonts.at(i)=NULL;
	}
	_fonts.clear();

	for (unsigned int i=0; i<_meshes.size(); i++)
	{
		static_cast<ID3DXMesh*>(_meshes.at(i)->mesh)->Release();
		delete _meshes.at(i);
		_meshes.at(i)=NULL;
	}
	_meshes.clear();

	_device->Release();
	_device=NULL;

	return false;
}