//-------------------------------------------------------------------------------
// file:	BackBuffer.h
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: This is the declaration for the backbuffer class to prevent the
//		flickering effect
//--------------------------------------------------------------------------

#ifndef BACKBUFFER_H
#define BACKBUFFER_H

#include <windows.h>

class BackBuffer
{
public:
	BackBuffer(HWND hWnd, int width, int height);
	~BackBuffer();	

	HDC		getDC();		// Returns the backbuffer's assosiated device context
	int		getWidth();		// Returns width of the backbuffer
	int		getHeight();	// Returns height of the backbuffer
	void	present();		// Write the back buffer in the main window

private:
	HWND    m_hWnd;			// The handle to the main window
	HDC     m_hDC;			// A handle to a system memory device context
	HBITMAP m_hSurface;		// The backbuffer bitmap
	HBITMAP m_hOldObject;	// Previous bitmap loaded by the device context (to restaure it)
	int     m_nWidth;		// Backbuffer width
	int     m_nHeight;		// Backbuffer height
};

#endif // BACKBUFFER_H