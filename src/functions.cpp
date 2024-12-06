#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <unordered_map>
#include "FoodInfo.hpp"
#include "language.hpp"
#include "ioUtils.hpp"
#include "openaiUtils.hpp"
#include "utils.hpp"
#include "functions.hpp"
#undef max
using namespace std;


void PrintFunctions(){
    cout << language_pack["View_current_foods"] << endl;
    cout << language_pack["Add_a_food"] << endl;
    cout << language_pack["Remove_a_food"] << endl;
    cout << language_pack["View_foods_nearing_expiration"] << endl;
    cout << language_pack["View_recommended_menu"] << endl;
    cout << language_pack["Settings"] << endl;;
    cout << language_pack["Exit_program"] << endl;
}


void PrintFoods(const vector<FoodInfo>& list){
    int max_len = GetOutputLength(language_pack["food"]); // 이름 최대 길이
    int max_digits = GetOutputLength(language_pack["count"]); // 개수 최대 자릿수
    int date_size = max(GetOutputLength(language_pack["date"]), 8); // 날짜 길이

    for (auto& food : list){
        // 이름 길이
        int len = GetOutputLength(food.name);
        if (len > max_len)
            max_len = len;
        // 개수 자릿수
        int digits = CountDigits(food.count);
        if (digits > max_digits)
            max_digits = digits;
    }

    cout << "|-";
    for (int i = 0; i < max_len; i++) cout << '-';
    cout << "-|-";
    for (int i = 0; i < max_digits; i++) cout << '-';
    cout << "-|-";
    for (int i = 0; i < date_size; i++) cout << '-';
    cout << "-|" << endl;

    cout << "| " << language_pack["food"];
    for (int i = 0; i < max_len - GetOutputLength(language_pack["food"]); i++) cout << ' ';
    cout << " | " << language_pack["count"];
    for (int i = 0; i < max_digits - GetOutputLength(language_pack["count"]); i++) cout << ' ';
    cout << " | " << language_pack["date"];
    for (int i = 0; i < date_size - GetOutputLength(language_pack["date"]); i++) cout << ' ';
    cout << " |" << endl;

    cout << "|-";
    for (int i = 0; i < max_len; i++) cout << '-';
    cout << "-|-";
    for (int i = 0; i < max_digits; i++) cout << '-';
    cout << "-|-";
    for (int i = 0; i < date_size; i++) cout << '-';
    cout << "-|" << endl;

    for (auto& food : list){
        cout << "| " << food.name;
        for (int i = 0; i < max_len - GetOutputLength(food.name); i++) cout << ' ';
        cout << " | " << food.count;
        for (int i = 0; i < max_digits - CountDigits(food.count); i++) cout << ' ';
        cout << " | " << food.date;
        for (int i = 0; i < date_size - 8; i++) cout << ' ';
        cout << " |" << endl;

        cout << "|-";
        for (int i = 0; i < max_len; i++) cout << '-';
        cout << "-|-";
        for (int i = 0; i < max_digits; i++) cout << '-';
        cout << "-|-";
        for (int i = 0; i < date_size; i++) cout << '-';
        cout << "-|" << endl;
    }
}


void AddFood(vector<FoodInfo>& list, string file_dir){
    string name;
    int count;
    int date;

    // 사용자 입력 받기
    name = CinName();
    count = CinCount();
    date = CinDate();

    // 파일에 입력
    ofstream file(file_dir, ios::app | ios::binary);
    file << '\n' << name << " " << count << " " << date;
    file.close();

    // 벡터에 추가
    FoodInfo line;
    line.name = name;
    line.count = count;
    line.date = date;
    list.push_back(line);

    unordered_map<string, string> replacements;
    replacements.insert(make_pair("{name}", name));
    replacements.insert(make_pair("{count}", to_string(count)));
    cout << ReplacePlaceHolders(language_pack["added"], replacements) << endl;
}


void DeleteFood(vector<FoodInfo>& list, string file_dir){
    string name;
    int count;

    // 사용자 입력 받기
    name = CinName();
    count = CinCount();

    // 벡터에서 제거
    vector<FoodInfo>::iterator iter = list.begin();
    vector<FoodInfo>::iterator list_size = list.end();
    for (auto& food : list){
        if (name.compare(food.name) == 0){
            // 입력한 수량이 현재 수량보다 많으면 오류 출력
            if (count > food.count){
                cout << language_pack["count_over"] << endl;
                return;
            }
            else if (count < food.count){
                food.count -= count;
                break;
            }
            else{ // count == food.count
                list.erase(iter);
                break;
            }
        }
        iter += 1;
    }

    // 입력한 음식이 벡터 내에 없으면 오류 출력
    if (iter == list_size){
        cout << language_pack["not_exists"] << endl;
        return;
    }

    // 파일에서 제거
    // 제거한 벡터를 파일에 덮어쓰기
    ofstream file(file_dir, ios::trunc | ios::binary);
    for (auto food : list){
        file << food.name << " " << food.count << " " << food.date << '\n';
    }
    file.close();

    unordered_map<string, string> replacements;
    replacements.insert(make_pair("{name}", name));
    replacements.insert(make_pair("{count}", to_string(count)));
    cout << ReplacePlaceHolders(language_pack["removed"], replacements) << endl;
}


void PrintExpiringFoods(const vector<FoodInfo>& list){
    int current_date = GetCurrentDate();
    int threshold = 3; // 유통기한 임박 기준

    // 유통기한 임박 음식 벡터를 생성하여 PrintFood() 함수로 보내준다
    vector<FoodInfo> expiring_food_list;
    for (auto food : list){
        if (food.date - threshold <= current_date)
            expiring_food_list.push_back(food);
    }
    PrintFoods(expiring_food_list);
}


void RecommendMenu(vector<FoodInfo>& list){
    string system_prompt = "당신은 고급 요리 전문가입니다. 주어진 재료를 활용해 대중적이고 복잡한 요리를 제안하십시오.  \
                            요리는 다음 조건을 충족해야 합니다:  \
                            1. 대중적이고 유명한 요리를 제안하십시오.  \
                            2. 추가로 필요한 재료를 제안할 수 있습니다.  \
                            3. 모든 재료를 활용할 필요는 없습니다.  \
                            4. 다양한 메인 요리를 제안하십시오.  \
                            제안된 레시피는 고급 레스토랑이나 요리 경연 대회에서 사용할 수 있는 수준이어야 합니다.  \
                            요리의 이름, 재료, 간단한 설명만 출력하십시오.  \
                            터미널 환경에서 출력될 것을 생각해서 마크다운 문법은 사용하지 마시오.  \
                            출력 포멧:\n\
                            ----------------------------------------\n\
                            요리 이름:...\n\
                            재료:...\n\
                            설명:...\n\
                            마지막에 출력 포멧에서 쓰인 구분선을 출력하십시오.  \
                            출력 언어는 " + language_pack["language"] + "입니다.  \
                            출력 포멧을 포함하여 출력되는 모든 언어를 " + language_pack["language"] + "로 작성하십시오.";

    // 질문 작성
    // 시스템 프롬프트가 적용되어 있으므로 음식 목록만 전달한다
    string query;
    for (auto food : list){
        query += food.name;
        query += " ";
    }

    // 질문 보내고 대답 받기
    string response = GetResponse(system_prompt, query);

    // 대답 출력
    cout << response << endl;
}


void ChangeSettings(){
    while (1){
        cout << language_pack["-settings-"] << endl;
        cout << language_pack["Back"] << endl;
        cout << language_pack["Language"] << endl;

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
            return;
        }
        else if (choice == 1){
            SetLanguage();
        }
        else{
            cout << language_pack["invalid_input"] << endl;
        }
    }
}