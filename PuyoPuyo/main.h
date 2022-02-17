//-------------------------------------------------------------------------------
// file:	main.h
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: Here are declarations of global variables and constants
//--------------------------------------------------------------------------

#ifndef MAIN_H
#define MAIN_H

#include <windows.h>
#include <iostream>
#include <string>
#include "PuyoPuyoGame.h"
#include "BackBuffer.h"
#include "Board.h"

HINSTANCE		g_hAppInst		= 0;									// The game instance
HWND			g_hMainWnd		= 0;									// The handle to the game window
HDC				g_hSpriteDC		= 0;									// The device context to draw the differents sprites
std::string		g_WndCaption	= "Puyo Puyo - Gameloft - Yosbi Alves"; // The caption bar message
const int		g_nWindowWidth	= 305;									// The width of the game field
const int		g_nWindowHeight	= 625;									// The heigh of the game field
PuyoPuyoGame*	g_puyopuyogame	= 0;									// The game class (it contain all the logic)
BackBuffer*		g_backbuffer	= 0;									// The backbuffer class

#endif