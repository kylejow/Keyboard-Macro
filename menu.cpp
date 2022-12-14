#include "menu.h"

int printSavedTargets(nlohmann::ordered_json& savedMacros){
    system("cls");
    if(savedMacros.size() == 0){
        cout << "No saved macros\n\n\n";
        system("pause");
        return 1;
    }
    int num = 1;
    for(auto i = savedMacros.begin(); i != savedMacros.end(); i++){
        cout << num++ << ". " << i.key() << "\n";
    }
    cout << "\n";
    return 0;
}

string chooseFromSaved(nlohmann::ordered_json savedMacros){
    int chosen = 0;
    int numSaved = savedMacros.size();
    vector<string> names;
    for(auto i = savedMacros.begin(); i != savedMacros.end(); i++){
        names.push_back(i.key());
    }
    system("cls");
    printSavedTargets(savedMacros);
    cin >> chosen;
    while(cin.fail() || chosen < 1 || chosen > numSaved){
        system("cls");
        printSavedTargets(savedMacros);
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
        cin >> chosen;
    }
    return names[chosen-1];
}