#pragma once
#include <string>

inline std::string trim(const std::string &str, const std::string delim = " \t\n\r") {
	unsigned int bpos = str.find_first_not_of(delim), epos = str.find_last_not_of(delim);
	if (epos < bpos || epos >= str.length()) return "";
	return str.substr(bpos, epos - bpos + 1);
}