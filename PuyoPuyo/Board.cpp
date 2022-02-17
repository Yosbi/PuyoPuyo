//-------------------------------------------------------------------------------
// file:	PuyoPuyoGame.h
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: This is the class for the game, it acts as a world-controller and
//		having the main game logic implemented
//--------------------------------------------------------------------------
#include "Board.h"

// This is the game board we are going to be using
BoardSlot GameBoard[nGameBoardHeight][nGameBoardWidth];

//=========================================================
// Name: setPos
// Desc: Set the position of this player's sphere
//=========================================================
void PlayerSphere::setPos(int x, int y)
{
	m_nX = x;
	m_nY = y;
}