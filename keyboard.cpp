//https://en.cppreference.com/w/cpp/thread/sleep_for
//https://www.asciitable.com/
//https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes

#include "keyboard.h"

nlohmann::ordered_json createBasic(void){
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
        printBasic(keys, holds, delays);
        keys.push_back(getCharInput(a));
        holds.push_back(getIntInput(b));
        delays.push_back(getIntInput(c));
        cout << "\n";
        if(getYNInput(d) == 'y') stop = true;
    }
    macro["keys"] = keys;
    macro["holds"] = holds;
    macro["delays"] = delays;
    macro["type"] = "basic";
    return macro;
}

nlohmann::ordered_json createAdvanced(void){
    VKs VKs;
    system("cls");
    nlohmann::ordered_json macro;
    string key = "Enter key: ",
           delay = "Enter  duration in milliseconds: ",
           input;
    vector<tuple<string, int>> inputs;
    cout << "WARNING: ENSURE EVERY KEY PRESS HAS A KEY RELEASE\n\n";
    system("pause");
    vector<int> pressed;
    while(1){
        system("cls");
        printAdv(inputs);
        cout << "1. Key press\n"
             << "2. Key release\n"
             << "3. Add delay\n"
             << "4. Release all keys\n"
             << "5. Finish and save\n\n";
        cin >> input;
        system("cls");
        if(input == "1"){
            int x = getVKInput(VKs);
            inputs.push_back({"press", x});
            pressed.push_back(x);
        }else if(input == "2"){
            int x = getVKInput(VKs);
            inputs.push_back({"release", x});
            for(auto it = pressed.begin(); it != pressed.end(); it++){
                if(*it == x){
                    pressed.erase(it);
                    break;
                }
            }
        }else if(input == "3"){
            inputs.push_back({"delay", getIntInput(delay)});
        }else if(input == "4"){
            for(auto it = pressed.begin(); it != pressed.end(); it++){
                inputs.push_back({"release", *it});
            }
            pressed.clear();
        }else if(input == "5"){
            system("cls");
            break;
        }else{
            continue;
        }
    }
    macro["macro"] = inputs;
    macro["type"] = "advanced";
    return macro;
}

void printBasic(vector<char>& keys, vector<int>& holds, vector<int>& delays){
    for(unsigned long long int i = 0; i < keys.size(); i++){
        cout << i*4+1 << ". " << keys[i] << " {down}\n"
             << i*4+2 << ". " << "Waiting " << holds[i]  << "ms\n"
             << i*4+3 << ". " << keys[i] << " {up}\n"
             << i*4+4 << ". " << "Waiting " << delays[i] << "ms\n";
    }
    cout << "\n";
    return;
}

void printAdv(vector<tuple<string, int>> inputs){
    for(unsigned long long int i = 0; i < inputs.size(); i++){
        auto type = std::get<0>(inputs[i]);
        auto x = std::get<1>(inputs[i]);
        if(type == "press"){
            cout << i+1 << ". "<< x << " {down}\n";
        }else if(type == "release"){
            cout << i+1 << ". "<< x << " {up}\n";
        }else if(type == "delay"){
            cout << i+1 << ". " << "Waiting " << x << "ms\n";
        }
    }
    cout << "\n";
    return;
}

int getVKInput(VKs& VKs){
    string prompt = "Enter key: ", input;
    cout << prompt;
    cin >> input;
    while(1){
        if(input.length() == 1){
            int str = (int)input[0];
            if(str >= 97 && str <= 122){//uppercase
                return str - 32;
            }else if(str >= 65 && str <= 90){//lowercase
                return str;
            }else if(str >= 48 && str <= 57){//number
                return str;
            }
        }else{
            int x = VKs.isVK(input);
            if(x > -1){
                return VKs.getArchiveElement(x);
            }
        }
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        clearLastLine();
        cout << prompt;
        cin >> input;
    }
    return 1;
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

void VKs::printArchive(void){
    system("cls");
    cout << "Keys:\n\n0-9\na-z\n";
    for(unsigned long long int i = 0; i < archive.size(); i++){
        cout << std::get<0>(archive[i]) << "\n";
    }
    cout << "\n\n";
    return;
}

int VKs::isVK(string input){//returns index of VK from archive, else return -1
    for(long long unsigned int i = 0; i < archive.size(); i++){
        if(input == std::get<0>(archive[i])){
            return i;//std::get<1>(archive[i]);
        }
    }
    return -1;
}

int VKs::getArchiveElement(int i){
    return std::get<1>(archive[i]);
}

VKs::VKs() : archive ({{"backspace", VK_BACK},
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
                        {"selectmedia", VK_LAUNCH_MEDIA_SELECT},
                        {"f1", VK_F1},
                        {"f2", VK_F2},
                        {"f3", VK_F3},
                        {"f4", VK_F4},
                        {"f5", VK_F5},
                        {"f6", VK_F6},
                        {"f7", VK_F7},
                        {"f8", VK_F8},
                        {"f9", VK_F9},
                        {"f10", VK_F10},
                        {"f11", VK_F11},
                        {"f12", VK_F12},
                        {"f13", VK_F13},
                        {"f14", VK_F14},
                        {"f15", VK_F15},
                        {"f16", VK_F16},
                        {"f17", VK_F17},
                        {"f18", VK_F18},
                        {"f19", VK_F19},
                        {"f20", VK_F20},
                        {"f21", VK_F21},
                        {"f22", VK_F22},
                        {"f23", VK_F23},
                        {"f24", VK_F24}
                        }){}
