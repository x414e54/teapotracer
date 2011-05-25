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
#ifndef DXRENDERER_H
#define DXRENDERER_H

#include "AntRenderer.h"

#include <d3dx9.h>

struct VERTEX
{
    float x, y, z;
	float u;
	float v;
};

class DXRenderer : public AntRenderer
{
public:
	void DrawMesh(UINT meshId, POVector3* pos, POVector3* or);
	void SetProjection();
	void SetView(POVector3* pos, POVector3* target);
	void BeginScene();
	void DrawSprite(UINT textureId, RECT* src, POVector3* pos, float sx, float sw, float rotation);
	void DrawText(UINT fontId, LPCWSTR string, RECT* dst, AntFontColorARGB* fontColor);
	void DrawQuad(UINT textureID, RECT* rect, POVector3* pos, POVector3* Or);
	void EndScene();
	bool Cleanup();
	bool Init(HWND hWnd, AntSettings settings);
	void AddFont(LPCWSTR typeFace, LONG width, LONG height);
	void AddTexture(LPCWSTR fileName);
	void AddMesh(LPCWSTR fileName);

protected:
	IDirect3DDevice9 * _device;
	ID3DXSprite* _sprite;
};

#endif