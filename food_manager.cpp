// 기능이 실행될 때마다 어떤 기능인지 출력 (설정처럼)
// 언어에 따른 시간 조정 필요
// openai api를 사용할 수 없을 때 처리 필요
//     ConnectApi() 함수에서 key의 입력을 참조자 기반 매개변수로 받는다.
//     api에 연결할 수 없으면 사용자에게 그대로 진행할 것인지 묻는다.
//     그대로 진행한다면 참조자 기반 매개변수로 받은 key를 ""로 바꾼다.
//     main() 함수에서 key가 ""일 경우 RecommendMenu() 함수를 실행시키지 않는다.

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

// 일본어 설정일 경우 exe를 실행시키면 몇개의 문자에서 ?가 나옴
//     윈도우 터미널 인코딩이 안맞아서 발생
//     chcp 65001 입력 -> 인코딩을 utf-8로 바꾼다
//     코드에서 설정할 수 있는지 확인 필요

// https://www.abctech.software/2015/06/26/i18n-20/

#include <iostream>
#include <vector>
#include <limits>
#include "nlohmann/json.hpp"
#include "features.hpp"
#include "fileUtils.hpp"
#include "FoodInfo.hpp"
#include "foodList.hpp"
#include "ioUtils.hpp"
#include "language.hpp"
#include "openaiUtils.hpp"
#undef max
using namespace std;
using json = nlohmann::json;

// const string food_list_dir <- fileUtils.hpp
// const string settings_dir <- fileUtils.hpp
// unordered_map<string, string> language_pack <- language.hpp
// vector<FoodInfo> food_list <- foodList.hpp

int main(){
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
        LoadLanguagePack(language);
        string locale = language_pack["locale"];
        setlocale(LC_ALL, locale.c_str());
    }

    // openai api 연결
    string key;
    cout << language_pack["Enter openai api key: "];
    cin >> key;
    cout << language_pack["Connect to openai api"] << endl;
    ConnectApi(key);

    // 음식 리스트 불러오기
    cout << language_pack["Load the food list"] << endl;
    ReadFoodList();

    PrintFunctions();
    while (1){
        // 입력
        int choice; // 사용자 입력을 저장하는 변수
        choice = CinInteger(language_pack["Select the desired feature (View features: 0): "]);
        if (choice == 0){
            PrintFunctions();
        }
        else if (choice == 1){
            PrintFoods(food_list);
        }
        else if (choice == 2){
            AddFood();
        }
        else if (choice == 3){
            DeleteFood();
        }
        else if (choice == 4){
            PrintExpiringFoods();
        }
        else if (choice == 5){
            RecommendMenu();
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