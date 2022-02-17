//-------------------------------------------------------------------------------
// file:	Sprite.cpp
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: The implementation of the class sprite
//--------------------------------------------------------------------------

#include "Sprite.h"
#include <cassert>

//=========================================================
// Name: Sprite
// Desc: constructor
//=========================================================
Sprite::Sprite(HINSTANCE hAppInst, std::string sImage, std::string sMask, float x, float y)
{
	m_hAppInst = hAppInst;
	
	// Saving the position of the sprite 
	m_nPosX = x;
	m_nPosY = y;

	// Load the bitmap resources.
	m_hImage = (HBITMAP) LoadImage( NULL, sImage.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_hMask  = (HBITMAP) LoadImage( NULL, sMask.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// Get the BITMAP structure for each of the bitmaps.
	GetObject(m_hImage, sizeof(BITMAP), &m_ImageBM);
	GetObject(m_hMask,  sizeof(BITMAP), &m_MaskBM);
}

//=========================================================
// Name: Sprite
// Desc: destructor
//=========================================================
Sprite::~Sprite()
{
	// Free the resources we created in the constructor.
	DeleteObject(m_hImage);
	DeleteObject(m_hMask);
}

//=========================================================
// Name: getWidth
// Desc: returns the width of the bitmap
//=========================================================
int Sprite::getWidth()
{
	return m_ImageBM.bmWidth;
}

//=========================================================
// Name: getHeight
// Desc: returns the height of the bitmap
//=========================================================
int Sprite::getHeight()
{
	return m_ImageBM.bmHeight;
}

//=========================================================
// Name: update
// Desc: Updates the position of the sprite
//=========================================================
void Sprite::update(float x, float y)
{
	m_nPosX = x;
	m_nPosY = y;
}

//=========================================================
// Name: draw
// Desc: draws the bitmap into the backbuffer
//=========================================================
void Sprite::draw(HDC hBackBufferDC, HDC hSpriteDC)
{
	int w = getWidth();
	int h = getHeight();

	// Select the mask bitmap.
	HGDIOBJ oldObj = SelectObject(hSpriteDC, m_hMask);

	// Draw the mask to the backbuffer with SRCAND (using AND operator) 
	BitBlt(hBackBufferDC, (int)m_nPosX, (int)m_nPosY, w, h, hSpriteDC, 0, 0, SRCAND); 

	// Now select the image bitmap.
	SelectObject(hSpriteDC, m_hImage);

	// Draw the image to the backbuffer with SRCPAINT (using OR operator)
	BitBlt(hBackBufferDC, (int)m_nPosX, (int)m_nPosY, w, h, hSpriteDC, 0, 0, SRCPAINT);

	// Restore the original bitmap object.
	SelectObject(hSpriteDC, oldObj);
}