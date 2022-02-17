//-------------------------------------------------------------------------------
// file:	PuyoPuyoGame.h
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: This is the class for the game, it acts as a world-controller
//		having the main game logic implemented
//--------------------------------------------------------------------------

#ifndef PUYOPUYOGAME_H
#define PUYOPUYOGAME_H

#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cstdio>
#include "Sprite.h"
#include "Board.h"

class PuyoPuyoGame
{
public:
	PuyoPuyoGame(HINSTANCE hAppInst, HWND hMainWnd);
	~PuyoPuyoGame();

	void			pause();
	void			update();
	void			draw(HDC hBackBufferDC, HDC hSpriteDC);
	void			rotate(SPHEREPOSITION wantedRot);
	void			move(SPHEREPOSITION wantedPos);
	bool			updateBoard();

private:	
	SPHERETYPE		getRandonSphere();	
	void			newPlayerSpheres();
	bool			canMove(SPHEREPOSITION wantedPos);
	bool			canRotate(SPHEREPOSITION wantedRot);
	SPHEREPOSITION	getNextPosition(SPHEREPOSITION wantedRot);
	bool			checkLinks();
	void			check(int x, int y, std::vector<BoardSlot*>& vecBoardSlot);

public:	
	bool			m_bPaused;				// To know if the game is paused
	bool			m_bGameOver;			// To know if it is game over

private:
	HINSTANCE		m_hAppInst;				// The game handle
	HWND			m_hMainWnd;				// The window handle

	Sprite*			m_GameBoard;			// Sprite for the game board
	Sprite*			m_RedSphere;			// Sprite for the game sphere red
	Sprite*			m_GreenSphere;			// Sprite for the game sphere green
	Sprite*			m_BlueSphere;			// Sprite for the game sphere blue
	Sprite*			m_YellowSphere;			// Sprite for the game sphere yellow

	PlayerSphere	m_playerSphere1;		// The first sphere controlled by the user
	PlayerSphere	m_playerSphere2;		// The second sphere controlled by the user

	int				m_nScore;				// the score of the player
	bool			m_bNewPlayerSpheres;	// This boolean is used to know if we need to set 
											// a new pair of spheres for the user to play	
	
};

#endif // PUYOPUYOGAME_H