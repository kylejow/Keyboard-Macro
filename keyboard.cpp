//https://en.cppreference.com/w/cpp/thread/sleep_for

#include "keyboard.h"

nlohmann::ordered_json createMacro(void){
    system("cls");
    nlohmann::ordered_json macro;
    string a = "Enter key: ",
           b = "Enter keypress duration in milliseconds: ",
           c = "Enter delay in milliseconds: ",
           d = "Exit and save? y/n: ";
    vector<char> keys;
    vector<int> holds;
    vector<int> delays;
    bool stop = false;
    while(!stop){
        system("cls");
        printMacro(keys, holds, delays);
        cout << "\n";
        keys.push_back(getCharInput(a));
        holds.push_back(getIntInput(b));
        delays.push_back(getIntInput(c));
        cout << "\n";
        if(getYNInput(d) == 'y') stop = true;
    }
    macro["keys"] = keys;
    macro["holds"] = holds;
    macro["delays"] = delays;
    return macro;
}

void printMacro(vector<char>& keys, vector<int>& holds, vector<int>& delays){
    for(unsigned long long int i = 0; i < keys.size(); i++){
        cout << keys[i]   << " {down}\n"
             << holds[i]  << "ms\n"
             << keys[i]   << " {up}\n"
             << delays[i] << "ms\n";
    }
    return;
}