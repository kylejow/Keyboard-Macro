//https://en.cppreference.com/w/cpp/thread/sleep_for

#include "keyboard.h"

nlohmann::ordered_json createMacro(void){
    nlohmann::ordered_json macro;
    string a = "Enter key: ",
           b = "Enter keypress duration: ",
           c = "Enter delay in milliseconds: ";
    vector<char> keys;
    vector<int> holds;
    vector<int> delays;
    string input;
    while(input != "done"){
        system("cls");
        printCurrent(keys, holds, delays);
        keys.push_back(getCharInput(a));

    }

    return macro;
}

void printCurrent(vector<char>& keys, vector<int>& holds, vector<int>& delays){
    for(unsigned long long int i = 0; i < delays.size(); i++){
        cout << keys[i]   << " {down}\n"
             << holds[i]  << "ms\n"
             << keys[i]   << " {up}\n"
             << delays[i] << "ms\n";
    }
    return;
}