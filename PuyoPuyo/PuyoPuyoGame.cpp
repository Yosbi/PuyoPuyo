//-------------------------------------------------------------------------------
// file:	PuyoPuyoGame.cpp
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: This is the class for the game, it acts as a world-controller and
//		having the main game logic implemented
//--------------------------------------------------------------------------

#include "PuyoPuyoGame.h"
#include "resource.h"

//=========================================================
// Name: PuyoPuyoGame
// Desc: constructor, init the initial game state
//=========================================================
PuyoPuyoGame::PuyoPuyoGame(HINSTANCE hAppInst, HWND hMainWnd)
{
	// Save input parameters.
	m_hAppInst    = hAppInst;
	m_hMainWnd    = hMainWnd;
	
	// Set seed for the randor number generator
	srand((unsigned int)time(0));

	// The game is initially paused.
	m_bPaused    = true;

	// The game is not initially over
	m_bGameOver = false;

	// Init the score
	m_nScore = 0;

	// Create the sprites:
	m_GameBoard		= new Sprite(m_hAppInst, "res/back.bmp"		, "res/backmask.bmp",	0.0f, 0.0f);
	m_BlueSphere	= new Sprite(m_hAppInst, "res/blue.bmp"		, "res/mask.bmp",		0.0f, 0.0f);
	m_GreenSphere	= new Sprite(m_hAppInst, "res/green.bmp"	, "res/mask.bmp",		0.0f, 0.0f);
	m_RedSphere		= new Sprite(m_hAppInst, "res/red.bmp"		, "res/mask.bmp",		0.0f, 0.0f);
	m_YellowSphere	= new Sprite(m_hAppInst, "res/yellow.bmp"	, "res/mask.bmp",		0.0f, 0.0f);

	// init the board
	for (int y = 0; y < nGameBoardHeight; y++)
	{
		for (int x = 0; x < nGameBoardWidth; x++)
		{
			GameBoard[y][x].m_spheretype = NONE;
			GameBoard[y][x].m_bChecked = false;
		}
	}

	newPlayerSpheres();
}

//=========================================================
// Name: PuyoPuyoGame
// Desc: destructor
//=========================================================
PuyoPuyoGame::~PuyoPuyoGame()
{
	delete m_GameBoard;
	delete m_BlueSphere;
	delete m_GreenSphere;
	delete m_RedSphere;
	delete m_YellowSphere;
}


//=========================================================
// Name: pause
// Desc: this method pause or unpause the game
//=========================================================
void PuyoPuyoGame::pause()
{
	m_bPaused = !m_bPaused;
}

//=========================================================
// Name: draw
// Desc: this method draws all the sprites in the backbuffer
//		 it draws depending on the GameBoard state
//=========================================================
void PuyoPuyoGame::draw(HDC hBackBufferDC, HDC hSpriteDC)
{
	// Draw the sprites:
	// Draw the game board
	m_GameBoard->draw(hBackBufferDC, hSpriteDC);

	// Draw the spheres
	for (int y = 2; y < nGameBoardHeight; y++)
	{
		for (int x = 0; x < nGameBoardWidth; x++)
		{
			// if they are in a visible position
			// transform the GameBoar's position in screen position
		    float nPosX = ( (float)x * 300) / 6;
			float nPosY = ( ( (float)y - 2) * 600) / 12;

			switch(GameBoard[y][x].m_spheretype)
			{
			case RED:
				m_RedSphere->update(nPosX, nPosY);
				m_RedSphere->draw(hBackBufferDC, hSpriteDC);
				break;
			case GREEN:
				m_GreenSphere->update(nPosX, nPosY);
				m_GreenSphere->draw(hBackBufferDC, hSpriteDC);
				break;
			case BLUE:
				m_BlueSphere->update(nPosX, nPosY);
				m_BlueSphere->draw(hBackBufferDC, hSpriteDC);
				break;
			case YELLOW:
				m_YellowSphere->update(nPosX, nPosY);
				m_YellowSphere->draw(hBackBufferDC, hSpriteDC);
				break;
			}
		}
	}

	// Draw the player's score
	char score[24];
	sprintf_s(score, "Score: %d", m_nScore);
	SetBkMode(hBackBufferDC, TRANSPARENT);
	TextOut(hBackBufferDC, 2, 0, score, (int)strlen(score));

	// If it is game over print the "game over" message
	if (m_bGameOver)
	{
		char msg1[] = "GAME OVER!";
		char msg2[] = "Press enter to play again!";	
		sprintf_s(score, "Your score: %d", m_nScore);
		TextOut(hBackBufferDC, (300 / 2) - 50, 150, msg1, (int)strlen(msg1));
		TextOut(hBackBufferDC, (300 / 2) - 85, 165, msg2, (int)strlen(msg2));
		TextOut(hBackBufferDC, (300 / 2) - 50, 180, score, (int)strlen(score));
	}

	// If it is paused print the "Game paused" message
	if (m_bPaused)
	{
		char msg1[] = "Game paused...";
		char msg2[] = "Press enter to play!";
		SetBkMode(hBackBufferDC, TRANSPARENT);
		TextOut(hBackBufferDC, (300 / 2) - 55, 150, msg1, (int)strlen(msg1));
		TextOut(hBackBufferDC, (300 / 2) - 70, 165, msg2, (int)strlen(msg2));
	}
}

//=========================================================
// Name: getRandonSphere
// Desc: this method returns a randon sphere color to put 
//		 in the start zone
//=========================================================
SPHERETYPE PuyoPuyoGame::getRandonSphere()
{
	return (SPHERETYPE)(1 + (rand() % 4)); // randon number between 1 and 4
}

//=========================================================
// Name: update
// Desc: This method handles the update of all the GameBoard.
//		 Also if both of the player's spheres has fallen down
//		 then it makes the call to check for spheres links
//=========================================================
void PuyoPuyoGame::update()
{
	if (!m_bGameOver)
	{
		if (!m_bPaused)
		{
			// If any of the player`s spheres still falling then
			if ((m_playerSphere1.m_bFalling) || (m_playerSphere2.m_bFalling))
			{
				updateBoard();
			}
		
			// When both of the player's spheres has fallen I check for the spheres's links
			if ((!m_playerSphere1.m_bFalling) && (!m_playerSphere2.m_bFalling))
			{
				// if there are no more updates on the board
				if (updateBoard() == false)
				{
					// and if there are no links
					if (checkLinks() == false)
					{
						m_bNewPlayerSpheres = true;
					}
				}
			}

			// if new pair of spheres is needed
			if (m_bNewPlayerSpheres)
			{
				// get a new pair of spheres for the player
				newPlayerSpheres();
			}
		}
	}
}

//=========================================================
// Name: checkLinks
// Desc: this method check if there are links in the spheres
//		 if there are, then it erase the balls and return 
//		 true
//=========================================================
bool PuyoPuyoGame::checkLinks()
{
	// if there has been a erase then this value will be true,
	// it is used as a return value to check if we need a new
	// pair of player's spheres or not
	bool bErase = false; 

	// check the gameboard from the botton to the top
	for (int y = nGameBoardHeight - 1; y >= 2; y--)
	{
		for (int x = 0; x < nGameBoardWidth; x++)
		{
			// If I have no checked this slot and is not empty
			if ((GameBoard[y][x].m_bChecked == false) && (GameBoard[y][x].m_spheretype != NONE))
			{
				// This vector is used to store the pointers to all the spheres of the same color that
				// we are checking now
				std::vector<BoardSlot*> vecBoardSlot;

				// Call this recursive method that will search for all the ocurrences of spheres of 
				// same color and then store them in the vector vecBoardSlot
				check(x, y, vecBoardSlot);			

				// if there are more than 4 slot of the same color, erase them
				if (vecBoardSlot.size() >= 4)
				{
					// there exitst at least one erase
					bErase = true;

					// this is the counter that will count the score that was achieved in this move
					int nScoreCounter = 0;

					for (std::vector<BoardSlot*>::iterator it = vecBoardSlot.begin(); it < vecBoardSlot.end(); it++)
					{
						(*it)->m_spheretype = NONE; // <-- erase the sphere
						nScoreCounter++;
					}

					// Add the score achieved by this move to the general score
					m_nScore += nScoreCounter - 3; // The score adds one from 4
				}
			}
		}
	}

	// return the board to a no-checked state
	for (int y = nGameBoardHeight - 1; y >= 0; y--)
	{
		for (int x = 0; x < nGameBoardWidth; x++)
		{
			GameBoard[y][x].m_bChecked = false;
		}
	}

	return bErase;
}

//=========================================================
// Name: check
// Desc: recursive method that will search for all the ocurrences of spheres of 
//	     same color and then store them in the vector vecBoardSlot
//=========================================================
void PuyoPuyoGame::check(int x, int y, std::vector<BoardSlot*>& vecBoardSlot)
{
	// first set this slot as checked
	GameBoard[y][x].m_bChecked = true;

	// Store the pointer to this slot in the vector
	vecBoardSlot.push_back(&GameBoard[y][x]);

	// Now I look for other nodes with the same color in this order: DOWN LEFT UP RIGHT
	// Looking RIGHT
	if (x < nGameBoardHeight - 1)
	{
		// if the down slot has not been checked and is the same color as this
		if ((GameBoard[y][x + 1].m_bChecked == false) && (GameBoard[y][x + 1].m_spheretype == GameBoard[y][x].m_spheretype))
		{
			// I check it 
			check(x + 1, y , vecBoardSlot);
		}
	}

	// Looking UP (must be less that two because the playable field beggins at the slot 2)
	if (y > 2)
	{
		// if the down slot has not been checked and is the same color as this
		if ((GameBoard[y - 1][x].m_bChecked == false) && (GameBoard[y - 1][x].m_spheretype == GameBoard[y][x].m_spheretype))
		{
			// I check it 
			check(x, y - 1 , vecBoardSlot);
		}
	}

	// Looking LEFT
	if (x > 0)
	{
		// if the down slot has not been checked and is the same color as this
		if ((GameBoard[y][x - 1].m_bChecked == false) && (GameBoard[y][x - 1].m_spheretype == GameBoard[y][x].m_spheretype))
		{
			// I check it 
			check(x - 1, y , vecBoardSlot);
		}
	}
	
	// Looking DOWN
	if (y < nGameBoardHeight - 1)
	{
		// if the down slot has not been checked and is the same color as this
		if ((GameBoard[y + 1][x].m_bChecked == false) && (GameBoard[y + 1][x].m_spheretype == GameBoard[y][x].m_spheretype))
		{
			// I check it 
			check(x, y + 1, vecBoardSlot);
		}
	}
}

//=========================================================
// Name: updateBoard
// Desc: This method updates the board, it returns true
//		 if there has been at least a movement on the board
//=========================================================
bool PuyoPuyoGame::updateBoard()
{
	// the movement only can occur when it is not paused
	if (m_bPaused)
		return false;

	bool bMovement = false;
	for (int y = nGameBoardHeight - 1; y >= 0 ; y--)
	{
		for (int x = 0; x < nGameBoardWidth; x++)
		{
			// if there is a space below this sphere and it is a blank space
			if ((y < nGameBoardHeight - 1) && (GameBoard[y + 1][x].m_spheretype == NONE) && (GameBoard[y][x].m_spheretype != NONE))
			{
				// there has been a movement
				bMovement = true;
				
				// move the ball down
				GameBoard[y + 1][x].m_spheretype = GameBoard[y][x].m_spheretype;
				GameBoard[y][x].m_spheretype = NONE;

				// if the updated is the sphere of the player
				if ((m_playerSphere1.m_nX == x) && (m_playerSphere1.m_nY == y))
				{
					m_playerSphere1.m_nY = y + 1;
				}
				if ((m_playerSphere2.m_nX == x) && (m_playerSphere2.m_nY == y))
				{
					m_playerSphere2.m_nY = y + 1;
				}
			}
			else // if there is not space in blank to move
			{
				// if this is any of the player's spheres then set the falling state to false
				if ((m_playerSphere1.m_nX == x) && (m_playerSphere1.m_nY == y))
				{
					m_playerSphere1.m_bFalling = false;
				}
				if ((m_playerSphere2.m_nX == x) && (m_playerSphere2.m_nY == y))
				{
					m_playerSphere2.m_bFalling = false;
				}
			}			
		}
	}
	return bMovement;
}

//=========================================================
// Name: newPlayerSpheres
// Desc: This method bring a new pair of random player's 
//		 spheres into the game, if it cannot put the 
//		 new pair then it is game over!
//=========================================================
void PuyoPuyoGame::newPlayerSpheres()
{
	// if there is still space to put spheres
	if ((GameBoard[1][2].m_spheretype == NONE) && (GameBoard[0][2].m_spheretype == NONE))
	{
		// Setting the first sphere to fall
		GameBoard[1][2].m_spheretype = getRandonSphere();
		GameBoard[0][2].m_spheretype = getRandonSphere();

		// saving the sphere of the user
		m_playerSphere1.setPos(2, 1);
		m_playerSphere1.m_bFalling = true;
		m_playerSphere1.m_position = DOWN;

		m_playerSphere2.setPos(2, 0);
		m_playerSphere2.m_bFalling = true;
		m_playerSphere2.m_position = UP;
	
		// Now we dont need a new pair
		m_bNewPlayerSpheres = false;
	}
	else
	{
		// The game is over!
		m_bGameOver = true;
	}
}

//=========================================================
// Name: rotate
// Desc: rotate the secondary sphere to the clockwise or
//		 counter clockwise depending of the user input
//=========================================================
void PuyoPuyoGame::rotate(SPHEREPOSITION wantedRot)
{
	// If the game is not paused
	if (m_bPaused)
		return;

	// Rotation can only happen if both player's sphere are falling
	if ( (!m_playerSphere1.m_bFalling) || (!m_playerSphere2.m_bFalling) )
		return;

	// getting the next position for the secondary sphere
	switch (getNextPosition(wantedRot))
	{
		case UP:
		{
			GameBoard[m_playerSphere1.m_nY - 1][m_playerSphere1.m_nX].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE,
			m_playerSphere2.setPos(m_playerSphere1.m_nX, m_playerSphere1.m_nY - 1);
			m_playerSphere2.m_position = UP;
			m_playerSphere1.m_position = DOWN;
			break;
		}
		case LEFT:
		{
			// if there is no room in the gameboard to rotate or there is a slot occupied
			if ((m_playerSphere2.m_nX == 0) || (GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX - 1].m_spheretype != NONE))
			{
				// check if the primary sphere can be moved to the right
				if (GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX + 1].m_spheretype == NONE)
				{
					GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX + 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
					GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
					GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE,
					m_playerSphere2.setPos(m_playerSphere1.m_nX, m_playerSphere1.m_nY);
					m_playerSphere2.m_position = LEFT;
					m_playerSphere1.m_nX++;
					m_playerSphere1.m_position = RIGHT;
				}				
			}
			else // rotate normally
			{				
				GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX - 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
				GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE,
				m_playerSphere2.setPos(m_playerSphere1.m_nX - 1, m_playerSphere1.m_nY);
				m_playerSphere2.m_position = LEFT;
				m_playerSphere1.m_position = RIGHT;				
			}
			break;
		}
		case DOWN:
		{
			// Check if the slot below is not ocuppied and if there is room in the gameboard
			if ((GameBoard[m_playerSphere1.m_nY + 1][m_playerSphere1.m_nX].m_spheretype == NONE) && (m_playerSphere1.m_nY < nGameBoardHeight - 1))
			{
				GameBoard[m_playerSphere1.m_nY + 1][m_playerSphere1.m_nX].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
				GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE,
				m_playerSphere2.setPos(m_playerSphere1.m_nX, m_playerSphere1.m_nY + 1);
				m_playerSphere2.m_position = DOWN;
				m_playerSphere1.m_position = UP;
			}
			break;
		}
		case RIGHT:
		{
			// if there is no room in the gameboard to rotate or there is a slot occupied
			if ((m_playerSphere2.m_nX == nGameBoardWidth - 1) || (GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX + 1].m_spheretype != NONE))
			{
				// check if the primary sphere can be moved to the left
				if (GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX - 1].m_spheretype == NONE)
				{
					GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX - 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
					GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
					GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE,
					m_playerSphere2.setPos(m_playerSphere1.m_nX, m_playerSphere1.m_nY);
					m_playerSphere2.m_position = RIGHT;
					m_playerSphere1.m_nX--;
					m_playerSphere1.m_position = LEFT;
				}				
			}
			else // rotate normally
			{
				GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX + 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
				GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE,
				m_playerSphere2.setPos(m_playerSphere1.m_nX + 1, m_playerSphere1.m_nY);
				m_playerSphere2.m_position = RIGHT;
				m_playerSphere1.m_position = LEFT;
				
			}
			break;
		}
	}
}

//=========================================================
// Name: getNextPosition
// Desc: this method returns the next position of the
//		 secondary player's sphere depending on the
//		 user's input
//=========================================================
SPHEREPOSITION PuyoPuyoGame::getNextPosition(SPHEREPOSITION wantedRot)
{
	switch (m_playerSphere2.m_position)
	{
		case UP:
		{
			if (wantedRot == LEFT)
				return LEFT;
			else
				return RIGHT;
			break;
		}
		case LEFT:
		{
			if (wantedRot == LEFT)
				return DOWN;
			else
				return UP;
			break;
		}
		case DOWN:
		{
			if (wantedRot == LEFT)
				return RIGHT;
			else
				return LEFT;
			break;
		}
		case RIGHT:
		{
			if (wantedRot == LEFT)
				return UP;
			else
				return DOWN;
			break;
		}
	}
	return wantedRot;
}

//=========================================================
// Name: move
// Desc: move the spheres right or left depending on the
//	     user's input
//=========================================================
void PuyoPuyoGame::move(SPHEREPOSITION wantedPos)
{
	// If the game is not paused
	if (m_bPaused)
		return;

	// left or right movement can only happen if both player's sphere are falling
	if ( (!m_playerSphere1.m_bFalling) || (!m_playerSphere2.m_bFalling) )
		return;

	// if movement is left	
	if ((wantedPos == LEFT) && (canMove(LEFT)))
	{
		// if sphere1 is left then move it first
		if (m_playerSphere1.m_position == LEFT)
		{
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX - 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX - 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE;
		}
		// if the sphere 2 is left the move it first 
		else if (m_playerSphere2.m_position == RIGHT)
		{
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX - 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX - 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype = NONE;
		}	
		// if one is above the other it does not matter the order of movement
		else 
		{
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX - 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX - 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype = NONE;
		}
		m_playerSphere1.m_nX--;
		m_playerSphere2.m_nX--;
	}
	// if movement is right	
	else if ((wantedPos == RIGHT) && (canMove(RIGHT)))
	{
		// if sphere1 is left then move it first
		if (m_playerSphere1.m_position == RIGHT)
		{
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX + 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX + 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE;
		}
		// if the sphere 2 is left the move it first
		else if (m_playerSphere2.m_position == RIGHT)
		{
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX + 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX + 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype = NONE;
		}		
		// if one is above the other it does not matter the order of movement
		else 
		{
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX + 1].m_spheretype = GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype;
			GameBoard[m_playerSphere2.m_nY][m_playerSphere2.m_nX].m_spheretype = NONE;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX + 1].m_spheretype = GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype;
			GameBoard[m_playerSphere1.m_nY][m_playerSphere1.m_nX].m_spheretype = NONE;
		}
		 m_playerSphere1.m_nX++;
		 m_playerSphere2.m_nX++;
	}
}

//=========================================================
// Name: canMove
// Desc: return true if the player's spheres can be moved
//		 left or right (depending on the user's input)
//=========================================================
bool PuyoPuyoGame::canMove(SPHEREPOSITION wantedPos)
{
	int x1 = m_playerSphere1.m_nX; int y1 = m_playerSphere1.m_nY;
	int x2 = m_playerSphere2.m_nX; int y2 = m_playerSphere2.m_nY;
	
	// if the wanted pos is right
	if (wantedPos == LEFT)
	{
		// If the spheres will not go outside boundaries
		if ((x1 > 0 ) && (x2 > 0 ))
		{
			// If any of the balls dont colide with other ball ignoring themselves			
			if ( ( (GameBoard[y1][x1 - 1].m_spheretype == NONE) || ((y1 == y2) && (x1 - 1 == x2)) ) &&				
				 ( (GameBoard[y2][x2 - 1].m_spheretype == NONE) || ((y1 == y2) && (x2 - 1 == x1)) ) )
			{
				return true;
			}
		}
	}
	else if (wantedPos == RIGHT)
	{
		// If the spheres will not go outside boundaries
		if ((x1 < nGameBoardWidth - 1) && (x2 < nGameBoardWidth - 1))
		{
			// If any of the balls dont colide with other ball ignoring themselves			
			if ( ( (GameBoard[y1][x1 + 1].m_spheretype == NONE) || ((y1 == y2) && (x1 + 1 == x2)) ) &&				
				 ( (GameBoard[y2][x2 + 1].m_spheretype == NONE) || ((y1 == y2) && (x2 + 1 == x1)) ) )
			{
				return true;
			}
		}
	}

	return false;
}