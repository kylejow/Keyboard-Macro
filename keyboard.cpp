//https://en.cppreference.com/w/cpp/thread/sleep_for
//https://www.asciitable.com/
//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

#include "keyboard.h"

nlohmann::ordered_json createMacro(void){
    system("cls");
    nlohmann::ordered_json macro;
    string a = "Enter key: ",
           b = "Enter keypress duration in milliseconds: ",
           c = "Enter delay in milliseconds: ",
           d = "Finish and save? y/n: ";
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

void sendChar(char c, int duration){
    int code = (int)c;
    if(code >= (int)0x30 && code <= (int)0x5A){
        sendVK(code, duration);
    }else{
        vector<int> specialChar = {VK_SHIFT};
        switch (c)
        {
        case ' ':
            sendVK(0x21, duration);
            break;
        case '!':
            specialChar.push_back(0x31);
            sendVKCombo(specialChar, duration);
            break;
        // case '"':
        //     specialChar.push_back(0x31);
        //     sendVKCombo(specialChar, duration);
        //     break;
        case '#':
            specialChar.push_back(0x33);
            sendVKCombo(specialChar, duration);
            break;
        case '$':
            specialChar.push_back(0x34);
            sendVKCombo(specialChar, duration);
            break;
        case '%':
            specialChar.push_back(0x35);
            sendVKCombo(specialChar, duration);
            break;
        
        default:
            break;
        }
    }
}

void sendVK(int VK, int duration){
    INPUT down[1] = {0};
    down[0].type = INPUT_KEYBOARD;
    down[0].ki.wVk = VK;
    SendInput(ARRAYSIZE(down), down, sizeof(INPUT));
    Sleep(duration);
    INPUT up[1] = {0};
    up[0].type = INPUT_KEYBOARD;
    up[0].ki.wVk = VK;
    up[0].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(up), up, sizeof(INPUT));
    return;
}

void sendVKCombo(vector<int> VKs, int duration){
    for(auto i : VKs){
        INPUT down[1] = {0};
        down[0].type = INPUT_KEYBOARD;
        down[0].ki.wVk = i;
        SendInput(ARRAYSIZE(down), down, sizeof(INPUT));
    }
    Sleep(duration);
    for(auto i : VKs){
        INPUT up[1] = {0};
        up[0].type = INPUT_KEYBOARD;
        up[0].ki.wVk = i;
        up[0].ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(ARRAYSIZE(up), up, sizeof(INPUT));
    }
    return;
}