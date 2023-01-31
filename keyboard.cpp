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
        printAdv(inputs, VKs);
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

void printAdv(vector<tuple<string, int>> inputs, VKs VKs){
    for(unsigned long long int i = 0; i < inputs.size(); i++){
        auto type = std::get<0>(inputs[i]);
        auto x = std::get<1>(inputs[i]);
        if(type == "press"){
            cout << i+1 << ". "<< VKs.getArchiveElement(x) << " {down}\n";
        }else if(type == "release"){
            cout << i+1 << ". "<< VKs.getArchiveElement(x) << " {up}\n";
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
                return x;
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

void runBasic(vector<char>& keys, vector<int>& holds, vector<int>& delays){
    system("cls");
    cout << "Running...\n\n";
    for(unsigned long long int i = 0; i < keys.size(); i++){
        cout << keys[i]   << " {down}\n"
            << "Waiting " << holds[i]  << "ms\n";
        sendChar(keys[i], holds[i]);
        cout << keys[i]   << " {up}\n";
        cout << "Waiting " << delays[i] << "ms\n";
        Sleep(delays[i]);
    }
    return;
}

void runAdv(vector<tuple<string, int>>& inputs){
    for(unsigned long long int i = 0; i < inputs.size(); i++){
        auto type = std::get<0>(inputs[i]);
        auto x = std::get<1>(inputs[i]);
        if(type == "press"){
            cout << x << " {down}\n";
            sendVKDown(x);
        }else if(type == "release"){
            cout << x << " {up}\n";
            sendVKUp(x);
        }else if(type == "delay"){
            cout << "Waiting " << x << "ms\n";
            Sleep(x);
        }
    }
    return;
}

void VKs::printArchive(void){
    system("cls");
    cout << "Keys:\n\n0-9\na-z\n";
    for(unsigned long long int i = 0; i < archive.size(); i++){
        if(archive[i] != "") cout << archive[i] << "\n";
    }
    cout << "\n\n";
    return;
}

int VKs::isVK(string input){//returns index of VK from archive, else return -1
    for(long long unsigned int i = 0; i < archive.size(); i++){
        if(input == archive[i]){
            return i;//std::get<1>(archive[i]);
        }
    }
    return -1;
}

string VKs::getArchiveElement(int i){
    return archive[i];
}

VKs::VKs(){
    vector<string> temp(256);
    for(int i = 0x30; i < 0x5A; i++){
        temp[i] = (char)i;
    }
    temp[8] = "backspace";
    temp[9] = "tab";
    temp[12] = "clear";
    temp[13] = "enter";
    temp[16] = "shift";
    temp[17] = "control";
    temp[18] = "alt";
    temp[19] = "pause";
    temp[20] = "capslock";
    temp[27] = "esc";
    temp[32] = "space";
    temp[33] = "pageup";
    temp[34] = "pagedown";
    temp[35] = "end";
    temp[36] = "home";
    temp[37] = "left";
    temp[38] = "up";
    temp[39] = "right";
    temp[40] = "down";
    temp[41] = "select";
    temp[42] = "print";
    temp[43] = "execute";
    temp[44] = "printscreen";
    temp[45] = "insert";
    temp[46] = "delete";
    temp[47] = "help";
    temp[91] = "Lwindows";
    temp[92] = "Rwindows";
    temp[93] = "applications";
    temp[95] = "sleep";
    temp[112] = "f1";
    temp[113] = "f2";
    temp[114] = "f3";
    temp[115] = "f4";
    temp[116] = "f5";
    temp[117] = "f6";
    temp[118] = "f7";
    temp[119] = "f8";
    temp[120] = "f9";
    temp[121] = "f10";
    temp[122] = "f11";
    temp[123] = "f12";
    temp[124] = "f13";
    temp[125] = "f14";
    temp[126] = "f15";
    temp[127] = "f16";
    temp[128] = "f17";
    temp[129] = "f18";
    temp[130] = "f19";
    temp[131] = "f20";
    temp[132] = "f21";
    temp[133] = "f22";
    temp[134] = "f23";
    temp[135] = "f24";
    temp[144] = "numlock";
    temp[145] = "scrolllock";
    temp[160] = "Lshift";
    temp[161] = "Rshift";
    temp[162] = "Lcontrol";
    temp[163] = "Rcontrol";
    temp[164] = "Lalt";
    temp[165] = "Ralt";
    temp[166] = "browserback";
    temp[167] = "browserforward";
    temp[168] = "browserrefresh";
    temp[169] = "browserstop";
    temp[170] = "browsersearch";
    temp[171] = "browserfavorites";
    temp[172] = "browserhome";
    temp[173] = "volumemute";
    temp[174] = "volumedown";
    temp[175] = "volumeup";
    temp[176] = "nextmedia";
    temp[177] = "previousmedia";
    temp[178] = "stopmedia";
    temp[179] = "playpausemedia";
    temp[180] = "launchmail";
    temp[181] = "selectmedia";
    temp[246] = "attn";
    temp[247] = "crsel";
    temp[VK_ATTN] = "attn";
    temp[VK_CRSEL] = "CrSel";
    temp[VK_EREOF] = "eraseEOF";
    temp[VK_PLAY] = "play";
    temp[VK_ZOOM] = "zoom";
    temp[VK_CLEAR] = "clear";
    archive = temp;
}
