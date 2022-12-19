//https://en.cppreference.com/w/cpp/thread/sleep_for

#include "keyboard.h"

nlohmann::ordered_json createMacro(void){
    nlohmann::ordered_json macro;
    string a = "Enter key: ",
           b = "Enter keypress duration in milliseconds: ",
           c = "Enter delay in milliseconds: ",
           d = "Exit and save? y/n: ";
    vector<char> keys;
    vector<int> holds;
    vector<int> delays;
    string stop;
    keys.push_back(getCharInput(a));
    holds.push_back(getIntInput(b));
    delays.push_back(getIntInput(c));
    while(1){
        system("cls");
        printCurrent(keys, holds, delays);
        if(getYNInput(d) == 'y') break;
        //clearLastLine();
        keys.push_back(getCharInput(a));
        holds.push_back(getIntInput(b));
        delays.push_back(getIntInput(c));
    }
    return macro;
}

void printCurrent(vector<char>& keys, vector<int>& holds, vector<int>& delays){
    for(unsigned long long int i = 0; i < keys.size(); i++){
        cout << keys[i]   << " {down}\n"
             << holds[i]  << "ms\n"
             << keys[i]   << " {up}\n"
             << delays[i] << "ms\n";
    }
    return;
}