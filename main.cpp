/*References
get key state   https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getkeystate
INPUT           https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-input
send input      https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
key codes       https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
*/

#include <windows.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>
#include <vector>

#include "display.h"
#include "json.hpp"
#include "keyboard.h"
#include "menu.h"

using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::ref;

int main(){
    string filename, prompt;
    nlohmann::ordered_json savedMacros;
    std::ifstream load("profile.json");
    if(load){
        savedMacros = nlohmann::ordered_json::parse(load);
        saveToFile("backup.json", savedMacros);
    }
    load.close();
    string input;
    int startDelay = 1500;
    while(1){
        system("cls");
        cout << "1. Run saved macro\n"
             << "2. Create new macro\n"
             << "3. View macro list\n"
             << "4. Print macro\n"
             << "5. Edit macro delays\n"
             << "6. Delete macro\n"
             << "7. Load profile\n"
             << "8. Export profile\n"
             << "9. Change macro execute delay (current = " << startDelay << "ms)\n"
             << "10. View key archive\n"
             << "\n\nq to quit\n\n";
        cin >> input;
        system("cls");
        if(input == "1"){
            if(printSavedTargets(savedMacros)){
                continue;
            }
            string name = chooseFromSaved(savedMacros);
            system("cls");
            if(savedMacros[name]["type"] == "basic"){
                vector<char> keys = savedMacros[name]["keys"];
                vector<int> holds = savedMacros[name]["holds"];
                vector<int> delays = savedMacros[name]["delays"];
                system("cls");
                Sleep(startDelay);
                cout << "Running...\n\n";
                for(unsigned long long int i = 0; i < keys.size(); i++){
                    cout << keys[i]   << " {down}\n"
                        << "Waiting " << holds[i]  << "ms\n";
                    sendChar(keys[i], holds[i]);
                    cout << keys[i]   << " {up}\n";
                    cout << "Waiting " << delays[i] << "ms\n";
                    Sleep(delays[i]);
                }
            }else{// if(savedMacros[name]["type"] == "advanced"){
                vector<tuple<string, int>> inputs = savedMacros[name]["macro"];
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
            }
            
            
            system("pause");
            system("cls");
        }else if(input == "2"){
            string name, blank = "";
            cout << "Enter macro name: ";
            cin >> name;
            system("cls");
            cout << "1. Create basic macro (recommended)\n"
                 << "2. Create advanced macro\n\n\n";
            int macroType = getIntInRange(blank, 1, 2);
            switch(macroType){
            case 1:
                savedMacros[name] = createBasic();
                break;
            case 2:
                savedMacros[name] = createAdvanced();
            default:
                break;
            }
            saveToFile("profile.json", savedMacros);
        }else if(input == "3"){
            if(printSavedTargets(savedMacros)){
                continue;
            }
            system("pause");
            continue;
        }else if(input == "4"){
            if(printSavedTargets(savedMacros)){
                continue;
            }
            string name = chooseFromSaved(savedMacros);
            system("cls");
            if(savedMacros[name]["type"] == "basic"){
                vector<char> keys = savedMacros[name]["keys"];
                vector<int> holds = savedMacros[name]["holds"];
                vector<int> delays = savedMacros[name]["delays"];
                printBasic(keys, holds, delays);
            }else{
                vector<tuple<string, int>> inputs = savedMacros[name]["macro"];
                printAdv(inputs);
            }
            
            cout << "\n";
            system("pause");
        }else if(input == "5"){
            if(printSavedTargets(savedMacros)){
                continue;
            }
            string name = chooseFromSaved(savedMacros);
            system("cls");
            vector<char> keys = savedMacros[name]["keys"];
            vector<int> holds = savedMacros[name]["holds"];
            vector<int> delays = savedMacros[name]["delays"];
            int num = 1;
            for(unsigned long long int i = 0; i < keys.size(); i++){
                cout << "   "<< keys[i]   << " {down}\n";
                cout << num << ". " << "Waiting " << holds[i]  << "ms\n";
                num++;
                cout << "   " << keys[i]   << " {up}\n"
                     << num << ". " << "Waiting " << delays[i] << "ms\n";
                num++;
            }
            cout << "\n\n";
            int newTime = selectDelay(keys.size());
            prompt = "New delay in milliseconds: ";
            if(newTime%2){
                savedMacros[name]["holds"][newTime/2] = getIntInput(prompt);
            }else{
                savedMacros[name]["delays"][newTime/2-1] = getIntInput(prompt);
            }
            saveToFile("profile.json", savedMacros);
        }else if(input == "6"){
            if(printSavedTargets(savedMacros)){
                continue;
            }
            savedMacros.erase(chooseFromSaved(savedMacros));
            saveToFile("profile.json", savedMacros);
        }else if(input == "7"){
            cout << "Enter backup filename: ";
            cin >> filename;
            std::ifstream load(filename);
            if(load){
                savedMacros = nlohmann::ordered_json::parse(load);
                saveToFile("profile.json", savedMacros);
            }else{
                system("cls");
                cout << "File does not exist\n\n";
                system("pause");
            }
            load.close();
        }else if(input == "8"){
            cout << "Enter backup filename: ";
            cin >> filename;
            saveToFile(filename, savedMacros);
        }else if(input == "9"){
            prompt = "New macro execute delay in milliseconds: ";
            startDelay = getIntInput(prompt);
        }else if(input == "10"){
            VKs VKs;
            VKs.printArchive();
            system("pause");
        }else if(input == "q"){
            break;
        }
    }
    system("cls");
    return 0;
}
