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
    string filename;
    nlohmann::ordered_json savedMacros;
    std::ifstream load("profile.json");
    if(load){
        savedMacros = nlohmann::ordered_json::parse(load);
        saveToFile("backup.json", savedMacros);
    }
    load.close();
    string input;
    while(1){
        system("cls");
        cout << "1. Run saved macro\n"
             << "2. Create new macro\n"
             << "3. View macro list\n"
             << "4. View macro content\n"
             << "5. Edit macro\n"
             << "6. Delete macro\n"
             << "7. Load profile\n"
             << "8. Export profile\n"
             << "\n\nq to quit\n\n";
        cin >> input;
        system("cls");
        if(input == "1"){
            if(printSavedTargets(savedMacros)){
                continue;
            }
            string name = chooseFromSaved(savedMacros);
            
            // load

            system("cls");
            cout << "Running...\n\n";
            system("cls");
            system("pause");
        }else if(input == "2"){
            string name;
            cout << "Enter macro name: ";
            cin >> name;
            savedMacros[name] = createMacro();
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
            
            system("pause");
        }else if(input == "5"){
            if(printSavedTargets(savedMacros)){
                continue;
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
        }else if(input == "q"){
            break;
        }
    }
    system("cls");
    return 0;
}
