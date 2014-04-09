#include "windowControl.h"
#include <iostream>

windowControl::windowControl(const char *procName)
{
	procWnd = FindWindow(NULL,TEXT(procName));
	instantMouseMove = true;
}

windowControl::windowControl() {
	procWnd = NULL;
	instantMouseMove = true;
}

void windowControl::openWindow(const char* characterName) {
	char windowName[128] = "EVE - ";
	strcat_s(windowName, characterName);
	procWnd = FindWindow(NULL,TEXT(windowName));
}

void windowControl::clickMouse(coord co)
{
	SetForegroundWindow(procWnd);

	mouse_event(MOUSEEVENTF_LEFTDOWN, co.x, co.y, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, co.x, co.y, 0, 0);
}

void windowControl::rightClickMouse(coord co)
{
	SetForegroundWindow(procWnd);

	mouse_event(MOUSEEVENTF_RIGHTDOWN, co.x, co.y, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_RIGHTUP, co.x, co.y, 0, 0);
}

void windowControl::clickMouseExperimental(coord co) {
	RECT windowPosition;
	GetWindowRect(procWnd, &windowPosition);
	LPARAM lParam = (co.x /*+ windowPosition.left*/) + ((co.y /*+ windowPosition.top*/)<<16);
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	while(ptMouse.x < windowPosition.left || ptMouse.x > windowPosition.right || ptMouse.y < windowPosition.top || ptMouse.y > windowPosition.bottom) {
		Sleep(200);
		GetCursorPos(&ptMouse);
	}
	PostMessage(procWnd, WM_MOUSEMOVE, 0, lParam);
	Sleep(30);
	PostMessage(procWnd, WM_LBUTTONDOWN, 0, 0);
	Sleep(30);
	PostMessage(procWnd, WM_LBUTTONUP, 0, 0);
}

void windowControl::rightClickMouseExperimental(coord co) {
	RECT windowPosition;
	GetWindowRect(procWnd, &windowPosition);
	LPARAM lParam = (co.x /*+ windowPosition.left*/) + ((co.y /*+ windowPosition.top*/)<<16);
	POINT ptMouse;
	GetCursorPos(&ptMouse);
	while(ptMouse.x < windowPosition.left || ptMouse.x > windowPosition.right || ptMouse.y < windowPosition.top || ptMouse.y > windowPosition.bottom) {
		Sleep(200);
		GetCursorPos(&ptMouse);
	} 
	PostMessage(procWnd, WM_MOUSEMOVE, 0, lParam);
	Sleep(30);
	PostMessage(procWnd, WM_RBUTTONDOWN, 0, 0);
	Sleep(30);
	PostMessage(procWnd, WM_RBUTTONUP, 0, 0);
}

coord windowControl::lastPosSearchVert(coord c, int y) {

	//SetForegroundWindow(procWnd);
	RECT windowPosition;
	
	GetWindowRect(procWnd,&windowPosition);

	int finalX = (/*windowPosition.left + */c.x); 
	int finalY = (/*windowPosition.top + */c.y);

	int targetY = y;

    COLORREF firstColor, color;
    HDC hDC;
	coord lastPos;

	hDC = GetDC(procWnd);
	if (hDC == NULL)
		return coord();

	firstColor = GetPixel(hDC, finalX, finalY);
	if (firstColor == CLR_INVALID) 
		return coord();

	for(int n = 0; n < targetY; n++) {
		color = GetPixel(hDC, finalX, finalY + n);
		if (color == CLR_INVALID) {
			ReleaseDC(GetDesktopWindow(), hDC);
			return coord();
		}
		if(color != firstColor) {
			targetY = n + y;
			lastPos.set(finalX, finalY + n);
		}
	}
	
	ReleaseDC(GetDesktopWindow(), hDC);
	return lastPos;
}

bool windowControl::searchHor(coord c, int x) {

	//SetForegroundWindow(procWnd);
	RECT windowPosition;
	
	GetWindowRect(procWnd,&windowPosition);

	int finalX = (/*windowPosition.left + */c.x); 
	int finalY = (/*windowPosition.top + */c.y);

    COLORREF color, prevColor;
    HDC hDC;

    // Get the device context for the screen
	hDC = GetDC(procWnd);
	if (hDC == NULL)
		return false;

	// Retrieve the color at that position
	prevColor = GetPixel(hDC, finalX, finalY);
	if (prevColor == CLR_INVALID) 
		return false;

	for(int n = 0; n < x; n++) {
		color = GetPixel(hDC, finalX + n, finalY);
		if (color == CLR_INVALID) {
			ReleaseDC(GetDesktopWindow(), hDC);
			return false;
		}
		if(color != prevColor) {
			ReleaseDC(GetDesktopWindow(), hDC);
			return true;
		}
	}

	ReleaseDC(GetDesktopWindow(), hDC);

	return false;
}

bool windowControl::testModify(coord c, int ms) {
	//SetForegroundWindow(procWnd);
	RECT windowPosition;
	
	GetWindowRect(procWnd,&windowPosition);

	int finalX = (/*windowPosition.left + */c.x); 
	int finalY = (/*windowPosition.top + */c.y);

    COLORREF firstColor, secondColor;
    HDC hDC = GetDC(procWnd);
	if(!hDC)
		return false;

	clickMouseExperimental(coord(0, 0));

	Sleep(ms);

	firstColor = GetPixel(hDC, finalX, finalY);

	clickMouseExperimental(c);

	Sleep(ms);

	secondColor = GetPixel(hDC, finalX, finalY);
	ReleaseDC(GetDesktopWindow(), hDC);

	if(firstColor == secondColor) return false;
	return true;
	
}

void windowControl::moveMouse(coord co, int cursorSpeed)
{
	if(!procWnd) return;
	SetForegroundWindow(procWnd);
	Sleep(200);

	POINT ptMouse;
	RECT windowPosition;

	GetWindowRect(procWnd,&windowPosition);

	int finalX = (windowPosition.left + co.x); 
	int finalY = (windowPosition.top + co.y);

	GetCursorPos(&ptMouse);

	int mX = ptMouse.x;
	int mY = ptMouse.y;

	if(instantMouseMove) {
		mX = finalX;
		mY = finalY;
		SetCursorPos(finalX, finalY);
	}

	while(mX != finalX || mY != finalY)
	{
		
		GetCursorPos(&ptMouse);

		mX = ptMouse.x;
		mY = ptMouse.y;
		int dy = 0; int dx = 0;
		if(finalX > mX)
		{
			if(mX != finalX)
				dx = 1 + rand() % cursorSpeed;
		}
		else
		{
			if(mX != finalX)
				dx = -1 - rand() % cursorSpeed;
		}

		

		if(finalY > mY)
		{
			if(mY != finalY)
				dy = 1 + rand() % cursorSpeed;
		}
		else
		{
			if(mY != finalY)
				dy = -1 - rand() % cursorSpeed;
		}
		if(mX - dx <= finalX + (cursorSpeed + 1) && mX + dx >= finalX - (cursorSpeed + 1)) dx = finalX - mX;
		if(mY - dy <= finalY + (cursorSpeed + 1) && mY + dy >= finalY - (cursorSpeed + 1)) dy = finalY - mY;
		SetCursorPos(mX + dx ,mY + dy);

		//SetForegroundWindow(procWnd);
		Sleep(10);
	}

	if(instantMouseMove)
		Sleep(100);

}

void windowControl::pressKey(WORD key, bool controlDown, int x)
{
	
	SetForegroundWindow(procWnd);
	INPUT *controlKey = NULL;
	if(controlDown) {
		controlKey = new INPUT;
		controlKey->ki.wVk = 0x11;
		controlKey->type = INPUT_KEYBOARD;
		controlKey->ki.dwFlags = 0;
		controlKey->ki.time = 0;
		controlKey->ki.wScan = 0;
		controlKey->ki.dwExtraInfo = 0;
	}

	if(controlKey)
		SendInput(1,controlKey,sizeof(INPUT));

	for(int i = 0; i < x; i++)
	{
		INPUT *keyClick;
		keyClick = new INPUT;
		keyClick->ki.wVk = key;
		keyClick->type = INPUT_KEYBOARD;
		keyClick->ki.dwFlags = 0;
		keyClick->ki.time = 0;
		keyClick->ki.wScan = 0;
		keyClick->ki.dwExtraInfo = 0;

		SendInput(1,keyClick,sizeof(INPUT));
	
		Sleep(100);

		keyClick->ki.dwFlags = KEYEVENTF_KEYUP;

		SendInput(1,keyClick,sizeof(INPUT));
		delete keyClick;
	}


	if(controlKey) {
		controlKey->ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1,controlKey,sizeof(INPUT));
		delete controlKey;
	}

}

void windowControl::pressEnter() {
	unsigned char scanCode = MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC);
	LPARAM lParam = 1 | scanCode << 16;
	PostMessage(procWnd, WM_KEYDOWN, VK_RETURN, lParam);
	Sleep(20);
	lParam = lParam | 3 << 30;
	PostMessage(procWnd, WM_KEYUP, VK_RETURN, lParam);
}

void windowControl::sendChar(WORD key)
{

	PostMessage(procWnd, WM_CHAR, key, 0);

}

void windowControl::typeString(std::string str) {
	
	for (unsigned int n = 0; n < str.length(); n++) {
		WORD key = str.at(n);
		sendChar(key);
		Sleep(20);
	}
}

void windowControl::sendCommand(std::string cmd) {
	typeString(cmd);
	pressEnter();
}

windowControl::~windowControl(void)
{
}
