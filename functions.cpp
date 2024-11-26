#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "FoodInfo.hpp"
#include "constants.hpp"
#include "inputUtils.hpp"
#include "openaiUtils.hpp"
#include "utils.hpp"
#include "functions.hpp"
using namespace std;


void PrintFunctions(){
    cout << "1. 현재 음식 보기" << endl;
    cout << "2. 음식 추가" << endl;
    cout << "3. 음식 제거" << endl;
    cout << "4. 유통기한 임박 음식 보기" << endl;
    cout << "5. 추천 메뉴 보기" << endl;
    cout << "6. 프로그램 종료" << endl;
}


void PrintFoods(const vector<FoodInfo>& list){
    int max_len = 0; // 이름 최대 길이
    int max_digits = 0; // 개수 최대 자릿수
    int date_size = 8; // 날짜 길이 (8로 고정)

    for (auto& food : list){
        // 이름 길이
        int len = food.name.length();
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

    for (auto& food : list){
        cout << "| " << food.name;
        for (int i = 0; i < max_len - food.name.length(); i++) cout << ' ';
        cout << " | " << food.count;
        for (int i = 0; i < max_digits - CountDigits(food.count); i++) cout << ' ';
        cout << " | " << food.date << " |" << endl;

        cout << "|-";
        for (int i = 0; i < max_len; i++) cout << '-';
        cout << "-|-";
        for (int i = 0; i < max_digits; i++) cout << '-';
        cout << "-|-";
        for (int i = 0; i < date_size; i++) cout << '-';
        cout << "-|" << endl;
    }
}


void AddFood(vector<FoodInfo>& list){
    string name;
    int count;
    int date;

    // 사용자 입력 받기
    name = CinName();
    count = CinCount();
    date = CinDate();

    // 파일에 입력
    ofstream file(file_dir, ios::app);
    file << '\n' << name << ", " << count << ", " << date;
    file.close();

    // 벡터에 추가
    FoodInfo line;
    line.name = name;
    line.count = count;
    line.date = date;
    list.push_back(line);

    cout << name << " " << count << "개가 추가되었습니다." << endl;
}


void DeleteFood(vector<FoodInfo>& list){
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
                cout << "입력한 수량이 현재 수량을 초과합니다" << endl;
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
        cout << "해당 음식이 없습니다" << endl;
        return;
    }

    // 파일에서 제거
    // 제거한 벡터를 파일에 덮어쓰기
    ofstream file(file_dir);
    for (auto food : list){
        file << food.name << ", " << food.count << ", " << food.date << '\n';
    }
    file.close();

    cout << name << " " << count << "개가 제거되었습니다." << endl;
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
                            마지막에 출력 포멧에서 쓰인 구분선을 출력하십시오.";

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