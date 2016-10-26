// Project Celestial.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project Celestial.h"
#include "Overlord.h"
#include "KeyTranslation.h"
#include "CursorCodes.h"
#include <windowsx.h>
#include <thread>
#include <time.h>

using namespace Logic;
using namespace CrossHandlers;

#define MAX_LOADSTRING 100

// Global Variables:
HCURSOR* cursors;
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
Overlord* overlord;
CrossHandlers::Message* messageBuffer;
int currentMsg;
int maxBuffer = 1000;

bool gStop = false;
bool lStop = false;

unsigned int totalKeys = 10;

bool lHasStopped = false;
bool gHasStopped = false;
std::thread gThread;
std::thread lThread;

unsigned int lFrameRate = 0;
unsigned int lLastTime = 0;

unsigned int currentTime = 0;
unsigned int skipTime = 0;
unsigned int pauseTime = 0;
unsigned int stoppedTime = 0;
bool showCursor = true;
bool currentCursShow = true;
bool paused = false;

bool ignoreMouse = false;
short iMouseX;
short iMouseY;

HWND hWnd;

void dealWithCEMessages()
{

	CrossHandlers::Message* mess = overlord->GetNextSystemMessage();

	while (mess != nullptr)
	{

		if (mess->mess == SystemMess_SETCURSOR)
		{

			if (mess->params[0] == CursorCode_NONE)
			{

				if (currentCursShow && showCursor)
				{

					currentCursShow = false;
					ShowCursor(currentCursShow);

				}
			}
			else
			{

				SetCursor(cursors[mess->params[0]]);

				if (!currentCursShow)
				{

					currentCursShow = true;
					ShowCursor(currentCursShow);

				}

			}
		}
		else if (mess->mess == SystemMess_SHOWCURSOR || mess->mess == SystemMess_HIDECURSOR)
		{

			bool newShow = mess->mess == SystemMess_SHOWCURSOR;
			int state = 0;

			if (newShow != showCursor)
			{

				showCursor = newShow;
				state = ShowCursor(showCursor);

			}
		}
		else if (mess->mess == SystemMess_MOVECURSOR)
		{

			ignoreMouse = true;
			iMouseX = mess->params[0] | ((short)mess->params[1] << 8);
			iMouseY = mess->params[2] | ((short)mess->params[3] << 8);
			POINT pt;
			pt.x = iMouseX;
			pt.y = iMouseY;
			ClientToScreen(hWnd, &pt);

			SetCursorPos(pt.x, pt.y);

		}
		else if (mess->mess == SystemMess_PAUSE)
		{

			stoppedTime = max(currentTime, lLastTime- pauseTime);
			paused = mess->params[0] != 0;

		}
		else if (mess->mess == SystemMess_SKIP)
		{

			unsigned int skipTime;
			memcpy(&skipTime, mess->params, sizeof(unsigned int));
			pauseTime += skipTime;

		}

		mess = overlord->GetNextSystemMessage();

	}
}

int runLogic(Overlord* lordie)
{

	HRESULT hr = overlord->Init(hWnd);
	
	if (hr != S_OK)
	{

		lStop = true;
		gStop = true;

	}

	while (!lStop)
	{

		unsigned int time = clock();
		currentTime = time - pauseTime;

		if (time - lLastTime >= lFrameRate)
		{

			lordie->Update(paused ? stoppedTime : currentTime);

			if (paused)
			{

				pauseTime += time - lLastTime;

			}

			lLastTime = time;

		}
		else
		{

			std::this_thread::yield();

		}
	}

	overlord->Kill();
	gStop = true;

	while (!gHasStopped)
	{

		std::this_thread::yield();

	}

	delete overlord;
	lHasStopped = true;
	return 0;

}

unsigned int gFrameRate = 0;
unsigned int gLastTime = 0;

int runGraphics(Overlord* lordie)
{

	while (!gStop)
	{

		unsigned int time = clock();

		if (time - gLastTime >= gFrameRate)
		{

			gLastTime = time;
			lordie->HandleDrawing(time);

		}
	}

	gHasStopped = true;
	return 0;

}


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                    _In_ int       nCmdShow)
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(252019);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here
	cursors = nullptr;
	messageBuffer = new Message[maxBuffer];

	for (unsigned int i = 0; i < maxBuffer; i++)
	{

		messageBuffer[i].read = true;
		messageBuffer[i].destination = MessageSource_INPUT;
		messageBuffer[i].source = MessageSource_NA;
		messageBuffer[i].type = MessageType_INPUT;

	}

	currentMsg = 0;

	MSG msg = {0};
	HACCEL hAccelTable;
	overlord = new Overlord();

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_PROJECTCELESTIAL, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		//return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTCELESTIAL));
	// Main message loop:
	while(WM_QUIT != msg.message)
	{

		dealWithCEMessages();

		if( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			
			Sleep(0);
		}
	}

	//Deletes the overlord when the window is closed
	lStop = true;

	delete[] messageBuffer;
	delete[] cursors;

	while (!lHasStopped)
	{

		std::this_thread::yield();

	}

	return (int) msg.wParam;

}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTCELESTIAL));
	wcex.hCursor		= LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_PROJECTCELESTIAL);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

bool hasBorder = true;

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   cursors = new HCURSOR[CursorCode_NA];

   if (hasBorder)
   {
	   //(WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX)
	   hWnd = CreateWindow(szWindowClass, szTitle, (WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX), CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   }
   else
   {
	   
	   hWnd = CreateWindow(szWindowClass, szTitle,false ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_POPUP),CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
	   
   }

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   lStop = false;
   gStop = false;

   gThread = std::thread(runGraphics, overlord);
   lThread = std::thread(runLogic, overlord);

	gThread.detach();
	lThread.detach();

   //Gets drawingarea
   RECT rc;
   GetWindowRect(hWnd, &rc);
   long temp1 = rc.right - rc.left;
   long temp2 = rc.bottom - rc.top;

   int x = (GetSystemMetrics(SM_CXSCREEN) - temp1) / 2;
   int y = (GetSystemMetrics(SM_CYSCREEN) - temp2) / 2;
   /*SetWindowPos(hWnd, HWND_TOPMOST, x, y, temp1, temp2, SWP_NOSENDCHANGING);*/ 
   cursors[CursorCode_HAND] = LoadCursor(NULL, IDC_HAND);
   cursors[CursorCode_POINT] = LoadCursor(NULL, IDC_ARROW);
   cursors[CursorCode_LOAD] = LoadCursor(NULL, IDC_WAIT);
   cursors[CursorCode_CARET] = LoadCursor(NULL, IDC_IBEAM);

   return TRUE;

}

Input::CelestialKeyCategories translateKeyCategory(WPARAM keyParam)
{

	Input::CelestialKeyCategories retVal =
		keyParam == VK_SHIFT || 
		keyParam == VK_CONTROL ||
		keyParam == VK_CAPITAL ||
		keyParam == VK_MENU
		? Input::CelestialKeyCategories_MOD :

		keyParam == VK_ESCAPE || 
		(keyParam >= VK_F1 && keyParam <= VK_F12) || 
		(keyParam >= VK_PRIOR && keyParam <= VK_DOWN) ||
		keyParam == VK_INSERT ||
		keyParam == VK_DELETE ||
		keyParam == VK_CANCEL ||
		keyParam == VK_SCROLL ||
		keyParam == VK_SNAPSHOT
		? Input::CelestialKeyCategories_SPEC :

		(keyParam >= 0x30 && keyParam <= 0x39) ||
		(keyParam >= 0x41 && keyParam <= 0x5a) ||
		(keyParam >= VK_OEM_1 && keyParam <= VK_OEM_3) ||
		(keyParam >= VK_OEM_4 && keyParam <= VK_OEM_8) ||
		keyParam == VK_SPACE || 
		keyParam == VK_TAB ||
		keyParam == VK_OEM_102
		? Input::CelestialKeyCategories_CHAR : 
		Input::CelestialKeyCategories_NA;

	return retVal;

}

Message* getNextMessage()
{

	Message* messag = &messageBuffer[currentMsg];

	while (!messag->read)
	{

		std::this_thread::yield();

	}

	messag->read = false;
	currentMsg = (currentMsg + 1) % maxBuffer;
	return messag;

}
//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

//	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	MSG mess;
	mess.message = message;
	mess.wParam = wParam;
	mess.lParam = lParam;
	int x = 0; 
	int y = 0;

	unsigned char tempBuff[]{0, 0, 0, 0 ,0 ,0 ,0 ,0};
	bool up = false;
	bool right = false;
	bool mouse = false;
	bool charDown = false; 
	bool down = false;
	bool middle = false;
	bool mouseWheel = false;
	short xPar = GET_X_LPARAM(lParam);
	short yPar = GET_Y_LPARAM(lParam);
	Message* messag;

	switch (message)
	{
	case WM_SETCURSOR:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_WINDOWPOSCHANGING:

		if (hasBorder)
		{

			break;

		}

	case WM_SIZE:

		if (wParam != SIZE_MINIMIZED)
		{

			RECT rc;
			GetWindowRect(hWnd, &rc);
			long temp1 = rc.right - rc.left;
			long temp2 = rc.bottom - rc.top;

			int x = (GetSystemMetrics(SM_CXSCREEN) - temp1) / 2;
			int y = (GetSystemMetrics(SM_CYSCREEN) - temp2) / 2;
			SetWindowPos(hWnd, HWND_NOTOPMOST, x, y, temp1, temp2, SWP_NOSENDCHANGING);

		}

		break;
	case WM_CHAR:
		messag = getNextMessage();
		messag->mess = InputMess_CHAR;
		switch (wParam)
		{
		case 0x08:
			// Process a backspace. 
			tempBuff[0] = '\b';
			break;

		case 0x0A:
			// Process a linefeed. 
			tempBuff[0] = '\n';
			break;

		case 0x1B:
			// Process an escape. 
			break;

		case 0x09:
			// Process a tab. 
			tempBuff[0] = '\t';
			break;

		case 0x0D:
			// Process a carriage return. 
			tempBuff[0] = '\n';
			break;

		default:
			// Process displayable characters. 
			tempBuff[0] = wParam;
			break;
		}

		messag->SetParams(tempBuff, 0, 4);
		overlord->SendMsg(messag);
		break;
	case WM_MBUTTONUP:
		up = true;
	case WM_MBUTTONDOWN:
		middle = true;
		down = !up;
	case WM_RBUTTONUP:
		up = !down;
	case WM_RBUTTONDOWN:
		right = !middle;
		down = !up;
	case WM_LBUTTONUP:
		up = !down;
	case WM_MOUSEWHEEL:
		mouseWheel = !middle && !right && !up;
	case WM_LBUTTONDOWN:
		down = !up;
		mouse = true;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		up = !down;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		if ((lParam >> 30 == 0 || up) && !mouse)
		{
			
			Input::CelestialKeyCategories keyCat = translatedCategories[wParam];
			
			if (keyCat != Input::CelestialKeyCategories_NA)
			{

				messag = getNextMessage();
				messag->mess = up ? InputMess_KEYUP : InputMess_KEYDWN;
				tempBuff[0] = keyCat;
				tempBuff[1] = translatedKeys[wParam];
				messag->SetParams(tempBuff, 0, 2);
				overlord->SendMsg(messag);

			}
		}

		if (mouse)
		{

			Input::InputHandler::keyCode translatedKey = 
				right ? Input::InputHandler::keyCodeMouseR : 
				middle ? Input::InputHandler::keyCodeMouseM : 
				mouseWheel ? Input::InputHandler::keyCodeMouseW:
				Input::InputHandler::keyCodeMouseL;

			messag = getNextMessage();

			if (!mouseWheel)
			{

				messag->mess = up ? InputMess_MOUSEUP : InputMess_MOUSEDWN;
				tempBuff[0] = translatedKey >> 0;
				tempBuff[1] = translatedKey >> 8;
				tempBuff[2] = translatedKey >> 16;
				tempBuff[3] = translatedKey >> 24;

			}
			else
			{
				
				messag->mess = InputMess_MOUSEWHEEL;
				short delta = GET_WHEEL_DELTA_WPARAM(wParam);
				tempBuff[0] = delta >> 0;
				tempBuff[1] = delta >> 8;

			}

			messag->SetParams(tempBuff, 0, 4);
			overlord->SendMsg(messag);

		}
		break;
	case WM_MOUSEMOVE:

		if (ignoreMouse && iMouseX == xPar && iMouseY == yPar)
		{

			ignoreMouse = false;
			break;

		}

		if (xPar >= 0 && yPar >= 0)
		{

			messag = getNextMessage();
			messag->mess = InputMess_MOUSEMOVE;
			tempBuff[0] = xPar >> 0;
			tempBuff[1] = xPar >> 8;
			tempBuff[2] = yPar >> 0;
			tempBuff[3] = yPar >> 8;
			messag->SetParams(tempBuff, 0, 8);
			overlord->SendMsg(messag);
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	CoUninitialize();
	return 0;
}