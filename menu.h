#ifndef MENU_H
#define MENU_H

#include <windows.h>
#include <vector>
#include <iostream>

#include "json.hpp"

using std::string;
using std::cout;
using std::cin;
using std::vector;

int printSavedTargets(nlohmann::ordered_json& savedMacros);
string chooseFromSaved(nlohmann::ordered_json savedMacros);

#endif