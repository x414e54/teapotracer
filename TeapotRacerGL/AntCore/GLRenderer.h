//*****************************************************************************
//-----------------------------------------------------------------------------
// DxRenderer.h 
//
// Extends AntRenderer.h
//
// This is a DirectX renderer class.
//
// Currently Reset or Lost devices are not handled
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "AntRenderer.h"

//#include <d3dx9.h>

struct VERTEX
{
    float x, y, z;
	float u;
	float v;
};

class GLRenderer : public AntRenderer
{
public:
	void DrawMesh(UINT meshId, POVector3* pos, POVector3* o);
	void SetProjection();
	void SetView(POVector3* pos, POVector3* target);
	void BeginScene();
	void DrawSprite(UINT textureId, RECT* src, POVector3* pos, float sx, float sw, float rotation);
	void DrawText(UINT fontId, std::wstring string, RECT* dst, AntFontColorARGB* fontColor);
	void DrawQuad(UINT textureID, RECT* rect, POVector3* pos, POVector3* Or);
	void EndScene();
	bool Cleanup();
	bool Init(void* wnd, AntSettings settings);
	void AddFont(std::wstring typeFace, UINT width, UINT height);
	void AddTexture(std::wstring fileName);
	void AddMesh(std::wstring fileName);

protected:
	char* _device;
	//ID3DXSprite* _sprite;
};

#endif
