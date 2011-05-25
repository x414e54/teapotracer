//*****************************************************************************
//-----------------------------------------------------------------------------
// AntWrap.h 
//
// This wraps the creation of windows, and initalization of devices.
//-----------------------------------------------------------------------------
//*****************************************************************************

#pragma once
#ifndef ANTWRAP_H
#define ANTWRAP_H

#include "Ant.h"
#include "AntSettings.h"
#include "AntRenderer.h"
#include <SDL/SDL.h>
typedef void (* PRENDERCALLBACK)(float timeDelta);
typedef bool (* PMSGCALLBACK)(UINT msg, UINT p1, UINT p2);

void AntWrapSetRenderCallBack(PRENDERCALLBACK callback);
void AntWrapSetMsgCallBack(PMSGCALLBACK callback);

bool AntWrapInit(AntRenderer** device, AntSettings settings, wchar_t* wndName);

bool AntWrapQuit(AntRenderer* renderer);

int AntWrapRun();

//LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
