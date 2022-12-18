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
    cout << prompt;
    cin >> i;
    while(cin.fail()){
        system("cls");
        cout << prompt;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        cin >> i;
    }
    system("cls");
    return i;
}

char getCharInput(std::string& prompt){
    system("cls");
    cout << "sadinadsa\n";
    std::string str;
    cout << prompt;
    cin >> str;
    
    while(str.size() != 1){
        //system("cls");
        
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        clearLastLine();
        cout << prompt;
        cin >> str;
    }
    system("cls");
    return str[0];
}

template <typename T> void getInput(T& input, std::string& prompt){
    system("cls");
    cout << prompt;
    cin >> input;
    while(cin.fail()){
        system("cls");
        cout << prompt;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        cin >> input;
    }
    system("cls");
    return;
}

void saveToFile(std::string filename, nlohmann::ordered_json& savedMacros){
    std::ofstream save(filename);
    save << savedMacros.dump(1) + "\n";
    save.close();
}