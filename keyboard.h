#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <windows.h>
#include <atomic>
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <tuple>

#include "program.h"
#include "display.h"
#include "json.hpp"

using std::vector;
using std::thread;
using std::cout;
using std::string;
using std::tuple;

class VKs{
    public:
        VKs(void);
        void printArchive(void);
        int isVK(string input);
    private:
        const vector<tuple<string, int>> archive;
};

nlohmann::ordered_json createBasic(void);
nlohmann::ordered_json createAdvanced(void);
void printBasic(vector<char>& keys, vector<int>& holds, vector<int>& delays);
void printAdv(vector<tuple<string, int>> inputs);
int getVKInput(VKs& Vks);
void sendChar(char c, int duration);
void sendVK(int VK, int duration);
void sendVKCombo(vector<int> VKs, int duration);
void sendVKDown(int VK);
void sendVKUp(int VK);

#endif