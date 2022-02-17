//-------------------------------------------------------------------------------
// file:	main.cpp
// Author:	Yosbi Alves
// mail:	yosbi@outlook.com
// phone:	+584248157737
//-----------------------------------------------------------------------
//Desc: here is the entry point of the application, the main game loop,
//		the instantation of the window and the window procedure to listen 
//		to messages from the SO like the keyboard input.
//--------------------------------------------------------------------------
#include "main.h"

//=========================================================
// Name: WndProc
// Desc: The main window procedure this will handle the 
//		 input from the keyboard, the instantiation and
//		 deletion of the game class "PuyoPuyoGame.
//=========================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	switch( msg )
	{	
		// Create application resources.
		case WM_CREATE:
		{
			// Create the puyo puyo game
			g_puyopuyogame = new PuyoPuyoGame(g_hAppInst, hWnd);

			// Create system memory DC for the sprites
			g_hSpriteDC = CreateCompatibleDC(0);

			// Create the backbuffer.
			g_backbuffer = new BackBuffer(hWnd, g_nWindowWidth, g_nWindowHeight);

			return 0;
		}
	
		// key pressed
		case WM_KEYDOWN:
		{
			switch(wParam) 
			{ 
				case VK_RETURN:
					// if the game is not over then pause it
					if (!g_puyopuyogame->m_bGameOver)
					{
						g_puyopuyogame->pause();
					}
					else // create a new game
					{
						delete g_puyopuyogame;
						g_puyopuyogame = new PuyoPuyoGame(g_hAppInst, hWnd);
					}
					break;
				// Rotate left (counter clockwise). 
				case'A': 
					g_puyopuyogame->rotate(LEFT);
					break;
					
				// Rotate right (clockwise). 
				case'S': 	
					g_puyopuyogame->rotate(RIGHT);
					break;

				// Move left
				case VK_LEFT:
					g_puyopuyogame->move(LEFT);
					break;

				// Move Right
				case VK_RIGHT:
					g_puyopuyogame->move(RIGHT);
					break;

				// Move Down
				case VK_DOWN:
					g_puyopuyogame->updateBoard();
					break;

			}
			return 0;					
		}

		// Destroy application resources.
		case WM_DESTROY: 
		{
			delete g_puyopuyogame;
			delete g_backbuffer;
			DeleteDC(g_hSpriteDC);
			PostQuitMessage(0); 
			return 0;	
		}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//=========================================================
// Name: InitMainWindow
// Desc: Creates the main window upon which we will
//       draw the game graphics onto.  
//=========================================================
bool InitMainWindow()
{
	WNDCLASS wc; 
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= g_hAppInst;
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)::GetStockObject(NULL_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= "MyWndClassName";

	RegisterClass( &wc );

	// getting the screen resolution to put the window on the center of the screen
	RECT screensize;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &screensize);
	
	// Creating the window
	g_hMainWnd = ::CreateWindow("MyWndClassName", 
		g_WndCaption.c_str(), WS_OVERLAPPED | WS_SYSMENU, 
		(screensize.right / 2) - 150, (screensize.bottom / 2) - 300, 
		g_nWindowWidth, g_nWindowHeight, 0, 0, g_hAppInst, 0);

	if(g_hMainWnd == 0)
	{
		MessageBox(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	ShowWindow(g_hMainWnd, SW_NORMAL);
	UpdateWindow(g_hMainWnd);

	return true;
}

//=========================================================
// Name: Run
// Desc: Encapsulates the message loop and the gameloop.
//=========================================================
int Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	// Get the performance timer frequency.
	__int64 cntsPerSec	= 0;
	bool	perfExists	= QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec)!=0;
	if( !perfExists )
	{
		MessageBox(0, "Performance timer does not exist!", 0, 0);
		return 0;
	}

	double timeScale = 1.0 / (double)cntsPerSec;

	// Get the current time.
	__int64 lastTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	
	// there are two timers:
	double updateTime  = 0.0f;	// this one is to update the game and logics
	double renderTime  = 0.0f;	// this is to render the game


	while(msg.message != WM_QUIT)
	{
		// IF there is a Windows message then process it.
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		// ELSE, do game stuff.
		else
        {	
			// get the time now
			__int64 currTime = 0; 
			QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

			double deltaTime = (double)(currTime - lastTime) * timeScale;

			updateTime += deltaTime;
			renderTime += deltaTime;
			// Only update once every 70/100 seconds.
			if( updateTime >= 0.70f )
			{ 
				// Update the game
				g_puyopuyogame->update();

				// Reset timer
				updateTime = 0.0f;				
			}

			// render graphics at 60 frames per sec (it is good for this game)
			if (renderTime >= 0.016)
			{
				// Draw the game on the backbuffer
				g_puyopuyogame->draw(g_backbuffer->getDC(), g_hSpriteDC);
					
				// Present the backbuffer
				g_backbuffer->present();

				// Reset timer
				renderTime = 0.0;

				// Free processor for other tasks
				Sleep(16);
			}
			
			lastTime = currTime;
        }
    }
	// Return exit code back to operating system.
	return (int)msg.wParam;
}

//=========================================================
// Name: WinMain
// Desc: Program execution starts here.
//=========================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int showCmd)
{
	g_hAppInst = hInstance;

	// Create the main window.
	if( !InitMainWindow() )
	{
		MessageBox(0, "Window Creation Failed, can't init the game.", "Error", MB_OK);
		return 0;
	}
	
	// Enter the message loop.
	return Run();
}