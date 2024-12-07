// 초기화 기능 필요
// 정수 입력 리팩토링 필요
// 기능이 실행될 때마다 어떤 기능인지 출력 (설정처럼)
// settings.json에서 language가 이상한 문자열이면 setlocale에서 오류가 발생할 수 있음
// 언어에 따른 시간 조정 필요
// ConnectApi() 함수 리팩토링 필요

// 유통기한 지난 음식 처리 필요
//     1. 파일에서 자동적으로 삭제한다
//         일반적으로 유통기한 하루 이틀 지나도 냉장고에 있는 경우도 있다
//         따라서 냉장고에는 있지만 파일에는 없는 경우가 발생할 수 있다
//     2. 음식 처리는 전적으로 사용자에게 맡긴다
//         유통기한 임박 음식을 출력할 때 이미 지난 음식도 포함시킬지 생각해보아야 한다
//         프로그램을 시작할 때 유통기한이 지난 음식을 알려줄 수 있다.
// 예외처리 더 자세하게
// 뒤로가기 필요
// 음식의 이름이 같은 경우(같은 음식이지만 유통기한이 다른 경우) 처리 필요
// 음식 이름에 공백 있을 경우 오류 발생
// 유통기한으로 음식에 가중치를 부여하여 추천 메뉴를 받는것 고려
// 추천 메뉴 출력 시 제외할 음식 목록 필요함(냉장고에 간식이 있을 경우 이런것들을 포함하여 이상한 메뉴가 나올 수 있음)
// if-elseif-else 문으로 기능이나 설정을 선택하는 코드 함수화 가능한지 확인할것
// 칼로리 추가
// 기능 출력 항시 출력되도록 할 수 있는 설정 추가 필요
// 음식 이름이 설정된 언어랑 다르면 추천 음식을 제대로 출력을 못함

// https://www.abctech.software/2015/06/26/i18n-20/

#include <iostream>
#include <vector>
#include <limits>
#include "nlohmann/json.hpp"
#include "FoodInfo.hpp"
#include "language.hpp"
#include "fileUtils.hpp"
#include "openaiUtils.hpp"
#include "functions.hpp"
#undef max
using namespace std;
using json = nlohmann::json;

int main(){
    // 파일 경로
    string food_list_dir = "food_list.bin";
    string settings_dir = "settings.json";

    // 파일 존재 확인
    IsFile(food_list_dir);
    IsFile(settings_dir);

    // 설정 불러오기
    json settings = ReadJson(settings_dir);
    string language = settings["language"];
    if (language == ""){
        SetLanguage(true);
    }
    else{
        // 언어 팩 불러오기
        LoadLanguagePack(language);
        // locale 설정
        string locale = language_pack["locale"];
        setlocale(LC_ALL, locale.c_str());
    }

    // openai api 연결
    string key;
    cout << language_pack["Enter openai api key: "];
    cin >> key;
    cout << language_pack["Connect to openai api"] << endl;
    if (!ConnectApi(key)){
        cout << language_pack["Exit the program"] << endl;
        return 0;
    }

    // 음식 리스트 불러오기
    cout << language_pack["Load the food list"] << endl;
    vector<FoodInfo> food_list = ReadFoodList(food_list_dir);

    PrintFunctions();
    while (1){
        // 입력
        int choice; // 사용자 입력을 저장하는 변수
        cout << language_pack["Select the desired feature (View features: 0): "];

        cin >> choice;
        if (cin.fail() || cin.peek() != '\n'){
            cout << language_pack["Please enter an integer"] << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0){
            PrintFunctions();
        }
        else if (choice == 1){
            PrintFoods(food_list);
        }
        else if (choice == 2){
            AddFood(food_list, food_list_dir);
        }
        else if (choice == 3){
            DeleteFood(food_list, food_list_dir);
        }
        else if (choice == 4){
            PrintExpiringFoods(food_list);
        }
        else if (choice == 5){
            RecommendMenu(food_list);
        }
        else if (choice == 6){
            ChangeSettings();
        }
        else if (choice == 7){
            break;
        }
        else{
            cout << language_pack["Invalid input"] << endl;
        }
    }

    cout << language_pack["Exit the program"] << endl;
    return 0;
}