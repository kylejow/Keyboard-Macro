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
    int i;
    cout << prompt;
    cin >> i;
    while(cin.fail()){
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        clearLastLine();
        cout << prompt;
        cin >> i;
    }
    return i;
}

char getCharInput(std::string& prompt){
    std::string str;
    cout << prompt;
    cin >> str;
    while(str.size() != 1){
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        clearLastLine();
        cout << prompt;
        cin >> str;
    }
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

char getYNInput(std::string& prompt){
    std::string str;
    cout << prompt;
    cin >> str;
    while(1){
        if(str[0] == 'y' || str[0] == 'n') break;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        clearLastLine();
        cout << prompt;
        cin >> str;
    }
    return str[0];
}

void saveToFile(std::string filename, nlohmann::ordered_json& savedMacros){
    std::ofstream save(filename);
    save << savedMacros.dump(1) + "\n";
    save.close();
}

int selectDelay(int keys){
    string prompt = "\n\nSelect delay to edit: ";
    int i = INT_MIN;
    cout << prompt;
    cin >> i;
    while(cin.fail() || i ){
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        clearLastLine();
        cout << prompt;
        cin >> i;
    }
    return i;
}