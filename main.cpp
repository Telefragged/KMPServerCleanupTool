#include <iostream>
#include <string>
#include <regex>
#include <list>
#include <algorithm>
#include "Ship.h"
#include "windowControl.h"

int main(int argc, char *argv[]) {
	std::string curline;
	windowControl wnd("KMP Server 0.1.5.1");
	if (!wnd.procWnd) {
		std::cout << "Error opening window..\n";
		return 1;
	}

	std::list<Ship> shipList;

	std::regex shipIDreg("(.)*(Name: )(.)*(ID: )([a-f0-9-])*");
	std::regex shipDelReg("^([dD]){2}.*");

	while (std::getline(std::cin, curline)) {
		if (std::regex_match(curline, shipIDreg)) {
			Ship ship(curline);
			if (std::find(shipList.begin(), shipList.end(), ship) == shipList.end() && !regex_match(ship.shipName, shipDelReg)) {
				shipList.push_back(ship);
				std::cout << "New ship: " << ship.shipID << "\n";
			}
		}
		else if (curline.find("Unknown Command: /cleanup") != std::string::npos) {
			for (auto &ship : shipList) {
				wnd.sendCommand("/deleteship " + ship.shipID);
			}
			shipList.clear();
		}
		else {
			std::cout << curline << "\n";
		}
	}

	return 0;
}
