#ifndef PROGRAM_H
#define PROGRAM_H

#include <windows.h>
#include <atomic>
#include <string>
#include <limits>
#include <iostream>
#include <fstream>

#include "json.hpp"
#include "display.h"

using std::cout;
using std::cin;
using std::string;

void stopProgram(std::atomic_bool& stop);
int getIntInput(std::string& prompt);
char getCharInput(std::string& prompt);
char getYNInput(std::string& prompt);
void saveToFile(std::string filename, nlohmann::ordered_json& savedMacros);

#endif