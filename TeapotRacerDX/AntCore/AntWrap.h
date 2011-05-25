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

#include "AntSettings.h"
#include "AntRenderer.h"

typedef void (* PRENDERCALLBACK)(float timeDelta);
typedef bool (* PMSGCALLBACK)(UINT msg, WPARAM wParam, LPARAM lParam);

void AntWrapSetRenderCallBack(PRENDERCALLBACK callback);
void AntWrapSetMsgCallBack(PMSGCALLBACK callback);

bool AntWrapInit(AntRenderer** device, AntSettings settings, HINSTANCE hInstance, LPCWSTR wndName);

bool AntWrapQuit(AntRenderer* renderer);

int AntWrapRun();

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif