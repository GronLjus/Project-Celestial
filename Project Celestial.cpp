// Project Celestial.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project Celestial.h"
#include "Overlord.h"
#include <windowsx.h>
#include <thread>
#include <time.h>

using namespace Logic;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
Overlord* overlord;
CrossHandlers::Message* messageBuffer;
int currentMsg;
int maxBuffer = 100;

bool gStop = false;
bool lStop = false;
bool mStop = false;

unsigned int totalKeys = 10;

enum keyCode{
	keyCodeOmni = 0,
	keyCodeMod = 1,
	keyCodeAllChar = 2,
	keyCodeShift = 3,
	keyCodeControl = 4,
	keyCodeAlt = 5,

	keyCodeF1 = 6,
	keyCodeF2 = 7,
	keyCodeF3 = 8,
	keyCodeF4 = 9,
	keyCodeF5 = 10,
	keyCodeF6 = 11,
	keyCodeF7 = 12,
	keyCodeF8 = 13,
	keyCodeF9 = 14,
	keyCodeF10 = 15,
	keyCodeF11 = 16,
	keyCodeF12 = 17,

	keyCodeAnyMouse = 18,
	keyCodeMouseL = 19,
	keyCodeMouseR = 20,
	keyCodeNA
};

bool lHasStopped = false;
bool gHasStopped = false;
bool mHasStopped = false;
std::thread gThread;
std::thread lThread;
std::thread mThread;

unsigned int mFrameRate = 16;
unsigned int mLastTime = 0;

int runMessaging(Overlord* lordie)
{

	while (!mStop)
	{

		unsigned int time = clock();

		if (time - mLastTime >= mFrameRate)
		{

			mLastTime = time;
			lordie->UpdateMessages(time);

		}
		else
		{

			std::this_thread::yield();

		}
	}

	mHasStopped = true;
	return 0;

}

unsigned int lFrameRate = 0;
unsigned int lLastTime = 0;

HWND hWnd;

int runLogic(Overlord* lordie)
{

	HRESULT hr = overlord->Init(hWnd);
	mThread = std::thread(runMessaging, overlord);
	mThread.detach();
	
	if (hr != S_OK)
	{

		lStop = true;
		gStop = true;
		mStop = true;

	}

	while (!lStop)
	{

		unsigned int time = clock();

		if (time - lLastTime >= lFrameRate)
		{

			lLastTime = time;
			lordie->Update(time);

		}
		else
		{

			std::this_thread::yield();

		}
	}

	overlord->Kill();

	while (!gHasStopped || !mHasStopped)
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
	//_CrtSetBreakAlloc(39547);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here

	messageBuffer = new Message[maxBuffer];
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
	gStop = true;
	mStop = true;

	delete[] messageBuffer;

	while (!mHasStopped)
	{

		std::this_thread::yield();

	}

	lStop = true;

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
	wcex.hCursor		= nullptr;//LoadCursor(nullptr, IDC_ARROW);
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
   mStop = false;

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

   return TRUE;

}

keyCode translateKey(WPARAM keyParam)
{

	keyCode retVal = 
		keyParam == VK_SHIFT ? keyCodeShift :
		keyParam == VK_CONTROL ? keyCodeControl :
		keyParam == VK_MENU ? keyCodeAlt :

		keyParam == VK_F1 ? keyCodeF1 :
		keyParam == VK_F2 ? keyCodeF2 :
		keyParam == VK_F3 ? keyCodeF3 :
		keyParam == VK_F4 ? keyCodeF4 :
		keyParam == VK_F5 ? keyCodeF5 :
		keyParam == VK_F6 ? keyCodeF6 :
		keyParam == VK_F7 ? keyCodeF7 :
		keyParam == VK_F8 ? keyCodeF8 :
		keyParam == VK_F9 ? keyCodeF9 :
		keyParam == VK_F10 ? keyCodeF10 :
		keyParam == VK_F11 ? keyCodeF11 :
		keyParam == VK_F12 ? keyCodeF12 :

		keyParam == VK_LBUTTON ? keyCodeMouseL :
		keyParam == VK_RBUTTON ? keyCodeMouseR :
		keyCodeNA;

	return retVal;

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
	Message* messag = &messageBuffer[currentMsg];
	messag->source = MessageSource_NA;
	currentMsg = (currentMsg + 1) % maxBuffer;

	bool charDown = false;

	switch (message)
	{
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

		messag->mess = InputMess_CHARDOWN;
		messag->destination = MessageSource_INPUT;
		messag->param1 = wParam;
			
		overlord->SendMsg(messag);

		messag = &messageBuffer[currentMsg];
		messag->source = MessageSource_NA;
		currentMsg = (currentMsg + 1) % maxBuffer;

		messag->mess = InputMess_NONCHARDOWN;
		messag->destination = MessageSource_INPUT;
		messag->param1 = keyCodeAllChar;
		overlord->SendMsg(messag);
		charDown = true;

		messag = &messageBuffer[currentMsg];
		messag->source = MessageSource_NA;
		currentMsg = (currentMsg + 1) % maxBuffer;
		
	case WM_KEYDOWN:

		if (!charDown)
		{

			messag->mess = InputMess_NONCHARDOWN;
			messag->destination = MessageSource_INPUT;
			messag->param1 = translateKey(wParam);
			overlord->SendMsg(messag);

			if (wParam == VK_SHIFT ||
				wParam == VK_CONTROL ||
				wParam == VK_MENU ||
				wParam == VK_CAPITAL)
			{

				messag = &messageBuffer[currentMsg];
				messag->source = MessageSource_NA;
				currentMsg = (currentMsg + 1) % maxBuffer;

				messag->mess = InputMess_NONCHARDOWN;
				messag->destination = MessageSource_INPUT;
				messag->param1 = keyCodeMod;

			}
			else if (
				wParam == VK_LBUTTON ||
				wParam == VK_RBUTTON)
			{

				messag = &messageBuffer[currentMsg];
				messag->source = MessageSource_NA;
				currentMsg = (currentMsg + 1) % maxBuffer;

				messag->mess = InputMess_NONCHARDOWN;
				messag->destination = MessageSource_INPUT;
				messag->param1 = translateKey(wParam);

				messag = &messageBuffer[currentMsg];
				messag->source = MessageSource_NA;
				currentMsg = (currentMsg + 1) % maxBuffer;

				messag->mess = InputMess_NONCHARDOWN;
				messag->destination = MessageSource_INPUT;
				messag->param1 = keyCodeAnyMouse;

			}

			messag = &messageBuffer[currentMsg];
			messag->source = MessageSource_NA;
			currentMsg = (currentMsg + 1) % maxBuffer;

		}

		messag->mess = InputMess_NONCHARDOWN;
		messag->destination = MessageSource_INPUT;
		messag->param1 = keyCodeOmni;

		break;
	case WM_MOUSEMOVE:
		messag->mess = InputMess_MOUSEMOVE;
		messag->destination = MessageSource_INPUT;
		messag->param1 = GET_X_LPARAM(lParam);
		messag->param2 = GET_Y_LPARAM(lParam);
		overlord->SendMsg(messag);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	CoUninitialize();
	return 0;
}