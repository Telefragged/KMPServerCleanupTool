#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <string>

#define EVE_LIST_ITEM_SPACE 20

struct coord {
	int x;
	int y;

	coord(int xs, int ys) {
		x = xs;
		y = ys;
	}

	coord() {
		x = 0;
		y = 0;
	}

	void set(int xs, int ys) {
		x = xs;
		y = ys;
	}

	coord contextQuickbarOffset() {
		return coord(x + 30, y + 25);
	}

	coord contextMarketOffset() {
		return coord(x + 30, y + 55);
	}

	coord contextCancelOffset() {
		return coord(x + 30, y + 90);
	}

	coord contextModifyOffset() {
		return coord(x + 30, y + 10);
	}
};

class windowControl
{
public:
	windowControl(const char *);
	windowControl(void);
	void openWindow(const char *);
	void clickMouse(coord);
	void clickMouseExperimental(coord);
	void rightClickMouseExperimental(coord);
	void rightClickMouse(coord);
	void moveMouse(coord, int);
	void pressKey(WORD, bool, int);
	void pressEnter();
	void sendChar(WORD);
	void typeString(std::string);

	void sendCommand(std::string);

	coord lastPosSearchVert(coord, int);

	bool searchHor(coord, int);
	bool testModify(coord, int);

	~windowControl(void);

	bool instantMouseMove;

	HWND procWnd;
};