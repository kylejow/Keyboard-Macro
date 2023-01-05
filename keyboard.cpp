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
        cout << i*4+1 << ". "<< keys[i]   << " {down}\n"
             << i*4+2 << ". " << "Waiting " << holds[i]  << "ms\n"
             << i*4+3 << ". " << keys[i]   << " {up}\n"
             << i*4+4 << ". " << "Waiting " << delays[i] << "ms\n";
    }
    return;
}

void sendChar(char c, int duration){
    INPUT down[1] = {0};
    down[0].type = INPUT_KEYBOARD;
    down[0].ki.wVk = 0;
    down[0].ki.wScan = (short)c;
    down[0].ki.dwFlags = KEYEVENTF_UNICODE;
    SendInput(ARRAYSIZE(down), down, sizeof(INPUT));
    Sleep(duration);
    INPUT up[1] = {0};
    up[0].type = INPUT_KEYBOARD;
    up[0].ki.wVk = 0;
    up[0].ki.wScan = (short)c;
    up[0].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(up), up, sizeof(INPUT));
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

void sendVKDown(int VK){
    INPUT down[1] = {0};
    down[0].type = INPUT_KEYBOARD;
    down[0].ki.wVk = VK;
    SendInput(ARRAYSIZE(down), down, sizeof(INPUT));
    return;
}

void sendVKUp(int VK){
    INPUT up[1] = {0};
    up[0].type = INPUT_KEYBOARD;
    up[0].ki.wVk = VK;
    up[0].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(ARRAYSIZE(up), up, sizeof(INPUT));
    return;
}

void printArchive(void){
    vector<tuple<string, int>> archive = {{"backspace", VK_BACK},
                                          {"tab", VK_TAB},
                                          {"clear", VK_CLEAR},
                                          {"enter", VK_RETURN},
                                          {"shift", VK_SHIFT},
                                          {"control", VK_CONTROL},
                                          {"alt", VK_MENU},
                                          {"pause", VK_PAUSE},
                                          {"capslock", VK_CAPITAL},
                                          {"esc", VK_ESCAPE},
                                          {"pageup", VK_PRIOR},
                                          {"pagedown", VK_NEXT},
                                          {"end", VK_END},
                                          {"home", VK_HOME},
                                          {"left", VK_LEFT},
                                          {"up", VK_UP},
                                          {"right", VK_RIGHT},
                                          {"down", VK_DOWN},
                                          {"select", VK_SELECT},
                                          {"print", VK_PRINT},
                                          {"execute", VK_EXECUTE},
                                          {"printscreen", VK_SNAPSHOT},
                                          {"insert", VK_INSERT},
                                          {"delete", VK_DELETE},
                                          {"help", VK_HELP},
                                          {"Lwindows", VK_LWIN},
                                          {"Rwindows", VK_RWIN},
                                          {"applications", VK_APPS},
                                          {"sleep", VK_SLEEP},
                                          {"numlock", VK_NUMLOCK},
                                          {"scrolllock", VK_SCROLL},
                                          {"Lshift", VK_LSHIFT},
                                          {"Rshift", VK_RSHIFT},
                                          {"Lcontrol", VK_LCONTROL},
                                          {"Rcontrol", VK_RCONTROL},
                                          {"Lalt", VK_LMENU},
                                          {"Ralt", VK_RMENU},
                                          {"browserback", VK_BROWSER_BACK},
                                          {"browserforward", VK_BROWSER_FORWARD},
                                          {"browserrefresh", VK_BROWSER_REFRESH},
                                          {"browserstop", VK_BROWSER_STOP},
                                          {"browsersearch", VK_BROWSER_SEARCH},
                                          {"browserfavorites", VK_BROWSER_FAVORITES},
                                          {"browserhome", VK_BROWSER_HOME},
                                          {"volumemute", VK_VOLUME_MUTE},
                                          {"volumedown", VK_VOLUME_DOWN},
                                          {"volumeup", VK_VOLUME_UP},
                                          {"nextmedia", VK_MEDIA_NEXT_TRACK},
                                          {"previousmedia", VK_MEDIA_PREV_TRACK},
                                          {"stopmedia", VK_MEDIA_STOP},
                                          {"playpausemedia", VK_MEDIA_PLAY_PAUSE},
                                          {"launchmail", VK_LAUNCH_MAIL},
                                          {"selectmedia", VK_LAUNCH_MEDIA_SELECT}};
//f keys, a-z, 0-9
}