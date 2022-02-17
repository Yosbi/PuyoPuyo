//-------------------------------------------------------------------------------
// file:	BackBuffer.cpp
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: This is the implementation for the backbuffer class to prevent the
//		flickering effect
//--------------------------------------------------------------------------
#include "BackBuffer.h"

//=========================================================
// Name: BackBuffer
// Desc: constructor
//=========================================================
BackBuffer::BackBuffer(HWND hWnd, int width, int height)
{
	// Save a copy of the main window handle.
	m_hWnd = hWnd;

	// Get a handle to the device context associated with the window.
	HDC hWndDC = GetDC(hWnd);

	// Save the backbuffer dimensions.
	m_nWidth  = width;
	m_nHeight = height;

	// Create system memory device context that is compatible with the window one.
	m_hDC = CreateCompatibleDC(hWndDC);

	// Create the backbuffer surface bitmap	
	m_hSurface = CreateCompatibleBitmap(hWndDC, width, height);

	// Done with window DC.
	ReleaseDC(hWnd, hWndDC);

	// Init the backbuffer with white:
	// Select the backbuffer bitmap into the DC.
	m_hOldObject = (HBITMAP)SelectObject(m_hDC, m_hSurface);

	// Select a white brush.
	HBRUSH white    = (HBRUSH)GetStockObject(WHITE_BRUSH);
	HBRUSH oldBrush = (HBRUSH)SelectObject(m_hDC, white);

	// Clear the backbuffer rectangle.
	Rectangle(m_hDC, 0, 0, m_nWidth, m_nHeight);

	// Restore the original brush.
	SelectObject(m_hDC, oldBrush);
}

//=========================================================
// Name: BackBuffer
// Desc: destructor
//=========================================================
BackBuffer::~BackBuffer()
{
	SelectObject(m_hDC, m_hOldObject);
	DeleteObject(m_hSurface);
	DeleteDC(m_hDC);
}

//=========================================================
// Name: getDC
// Desc: returns the backbuffer's assosiated device context
//=========================================================
HDC BackBuffer::getDC()
{
	return m_hDC;
}

//=========================================================
// Name: getWidth
// Desc: returns width of the backbuffer
//=========================================================
int BackBuffer::getWidth()
{
	return m_nWidth;
}

//=========================================================
// Name: getHeight
// Desc: returns height of the backbuffer
//=========================================================
int BackBuffer::getHeight()
{
	return m_nHeight;
}

//=========================================================
// Name: present
// Desc: this method write the back buffer in the main window
//=========================================================
void BackBuffer::present()
{
	// Get a handle to the device context associated with the window.
	HDC hWndDC = GetDC(m_hWnd);

	// Copy the backbuffer contents over to the window client area.
	BitBlt(hWndDC, 0, 0, m_nWidth, m_nHeight, m_hDC, 0, 0, SRCCOPY);

	// Always free window DC when done.
	ReleaseDC(m_hWnd, hWndDC);
}

