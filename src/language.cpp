#include <iostream>
#include <limits>
#include "nlohmann/json.hpp"
#include "language.hpp"
#include "fileUtils.hpp"
#undef max
using namespace std;
using json = nlohmann::json;

unordered_map<string, string> language_pack;

void LoadLanguagePack(string language){
    string dir = "locales/.json";
    dir.insert(8, language);
    IsFile(dir);
    json lang_json = ReadJson(dir);
    for (auto& [key, value] : lang_json.items()) {
        language_pack[key] = value.get<string>();
    }
}


void SetLanguage(bool init){
    string language;
    if (init == true) LoadLanguagePack("en");

    while (1){
        if (init != true){
            cout << language_pack["-language-"] << endl;
            cout << language_pack["Back"] << endl;
        }
        cout << "1. English" << endl;
        cout << "2. Korean" << endl;

        int choice;
        cout << language_pack["select"];
        cin >> choice;
        if (cin.fail() || cin.peek() != '\n'){
            cout << language_pack["input_integer"] << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0){
            if (init == true){
                cout << language_pack["invalid_input"] << endl;
                continue;
            }
            return;
        }
        else if (choice == 1){
            language = "en";
            break;
        }
        else if (choice == 2){
            language = "ko";
            break;
        }
        else{
            cout << language_pack["invalid_input"] << endl;
            continue;
        }
    }

    // 설정 저장
    json settings = ReadJson("settings.json");
    settings["language"] = language;
    WriteJson("settings.json", settings);

    // 언어 팩 불러오기
    LoadLanguagePack(language);
    // locale 설정
    string locale = language_pack["locale"];
    setlocale(LC_ALL, locale.c_str());

    cout << language_pack["lang_completed"] << endl;
}