//-------------------------------------------------------------------------------
// file:	Board.h
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: In this file is defined all the data structures and enums that
//		will be part of the structure of the game board
//-----------------------------------------------------------------------

#ifndef BOARD_H
#define BOARD_H

//=========================================================
// Name: SPHERETYPE
// Desc: this enum is to define the color of the sphere
//		 that is in a defined slot at a defined time
//		 in the board matrix
//=========================================================
enum SPHERETYPE
{
	NONE	=	0,
	RED		=	1,
	GREEN	=	2,
	BLUE	=	3,
	YELLOW	=	4
};

//=========================================================
// Name: SPHEREPOSITION
// Desc: this enum is used to always know where are the 
//		 player's spheres in relation with each other
//=========================================================
enum SPHEREPOSITION
{
	UP,
	DOWN,
	LEFT, 
	RIGHT
};

//=========================================================
// Name: PlayerSphere
// Desc: This class is made to keep an eye of the sphere moved by
//		 by the player
//=========================================================
class PlayerSphere
{
public:
	int				m_nX;			// The positon of the sphere in x axis
	int				m_nY;			// The positon of the sphere in y axis
	bool			m_bFalling;		// This is the state of the sphere, if it is falling it will be true
	SPHEREPOSITION  m_position;		// The position of this sphere in relation with the other plyer's sphere

	void setPos(int x, int y);		// Set the position of this player's sphere
};

//=========================================================
// Name: BoardSlot
// Desc: this is the definition of each slot of the
//		 GameBoard matrix
//=========================================================
class BoardSlot
{
public:
	SPHERETYPE	m_spheretype;		// The type of ball we are going to render
	bool		m_bChecked;			// If I checked this slot looking for sphere's links
};

static const int nGameBoardWidth	= 6;						// The game board width
static const int nGameBoardHeight	= 14;						// The game board height (the visible board size is 12, it has two 
																// extra slots to render the new spheres outside the screen)
extern BoardSlot GameBoard[nGameBoardHeight][nGameBoardWidth];	// This is the game board we are going to be using

#endif // BOARD_H