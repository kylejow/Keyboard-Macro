#include "program.h"

void stopProgram(std::atomic_bool& stop){
    while(1){
        if((GetKeyState(VK_ESCAPE) & 0x8000)){
            stop = true;
            return;
        }
    }
}

int getIntInput(std::string& prompt){
    system("cls");
    int i;
    std::cout << prompt;
    std::cin >> i;
    while(std::cin.fail()){
        system("cls");
        std::cout << prompt;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        std::cin >> i;
    }
    system("cls");
    return i;
}

void saveToFile(std::string filename, nlohmann::ordered_json& savedMacros){
    std::ofstream save(filename);
    save << savedMacros.dump(1) + "\n";
    save.close();
}