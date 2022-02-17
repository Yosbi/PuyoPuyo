//-------------------------------------------------------------------------------
// file:	Sprite.h
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: This is the class that will contain the sprites
//--------------------------------------------------------------------------
#ifndef SPRITE_H
#define SPRITE_H

#include <windows.h>
#include <string>

class Sprite
{
public:
	Sprite(HINSTANCE hAppInst, std::string sImage, std::string sMask, float x, float y);
	~Sprite();

	int			getWidth();								// Returns the width of the bitmap
	int			getHeight();							// Returns the height of the bitmap
	void		update(float x, float y);				// Updates the position of the sprite
	void		draw(HDC hBackBufferDC, HDC hSpriteDC);	// Draws the bitmap into the backbuffer

public:
	float		m_nPosX;								// X position of the sprite
	float		m_nPosY;								// Y position of the sprite

protected:
	HINSTANCE	m_hAppInst;								// The handle to the game app
	HBITMAP		m_hImage;								// Handle to the sprite's bitmap resource
	HBITMAP		m_hMask;								// Handle to the mask of the sprite's bitmap
	BITMAP		m_ImageBM;								// The actual bitmap
	BITMAP		m_MaskBM;								// The actual mask
};

#endif // SPRITE_H