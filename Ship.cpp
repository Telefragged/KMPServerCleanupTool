#include "Ship.h"
#include "utils.h"
#include <iostream>

Ship::Ship()
{
	isDebris = false;
}

Ship::Ship(std::string shipStr) : Ship() {
	unsigned int nameStart = shipStr.find("Name: ");
	unsigned int nameEnd = shipStr.find("ID: ");
	shipName = trim(shipStr.substr(nameStart + 5, nameEnd - nameStart - 5));
	shipID = trim(shipStr.substr(nameEnd+4));
	if (shipName.find("Debris") != std::string::npos) isDebris = true;
}

Ship::~Ship()
{
}
