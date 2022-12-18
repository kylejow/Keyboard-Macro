#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <windows.h>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

#include "program.h"
#include "display.h"
#include "json.hpp"

using std::vector;
using std::thread;
using std::cout;
using std::string;

nlohmann::ordered_json createMacro(void);
void printCurrent(vector<char>& keys, vector<int>& holds, vector<int>& delays);

#endif