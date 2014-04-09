#pragma once
#include <string>

class Ship
{
public:
	std::string shipName, shipID;

	bool isDebris;

	bool operator==(const Ship & rhs) {
		return rhs.shipID == shipID;
	}

	Ship();
	Ship(std::string);
	~Ship();
};

