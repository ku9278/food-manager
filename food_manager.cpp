// 유통기한 지난 음식 처리 필요
//     1. 파일에서 자동적으로 삭제한다
//         일반적으로 유통기한 하루 이틀 지나도 냉장고에 있는 경우도 있다
//         따라서 냉장고에는 있지만 파일에는 없는 경우가 발생할 수 있다
//     2. 음식 처리는 전적으로 사용자에게 맡긴다
//         유통기한 임박 음식을 출력할 때 이미 지난 음식도 포함시킬지 생각해보아야 한다
//         프로그램을 시작할 때 유통기한이 지난 음식을 알려줄 수 있다.
// 예외처리 더 자세하게
// 잘못 입력했을 때 돌아갈 수 있는 방법이 필요함
// 음식의 이름이 같은 경우(같은 음식이지만 유통기한이 다른 경우) 처리 필요
// 음식 이름에 공백 있을 경우 오류 발생
// 유통기한으로 음식에 가중치를 부여하여 추천 메뉴를 받는것 고려
// 추천 메뉴 출력 시 제외할 음식 목록 필요함(냉장고에 간식이 있을 경우 이런것들을 포함하여 이상한 메뉴가 나올 수 있음)
// csv 직접 접근 불허 필요
//     csv를 한번 엑셀로 열면 콤마 사이의 공백이 없어져서 불러올 때 오류가 발생한다.
// csv말고 txt 사용하는것 고려
// ReadFoodListCsv 개선 필요 (14_1_FileIO_Exception.pdf)
// 초기화 기능 필요

#include <iostream>
#include <vector>
#include <limits>
#include "FoodInfo.hpp"
#include "fileUtils.hpp"
#include "openaiUtils.hpp"
#include "functions.hpp"
#undef max
using namespace std;

int main(){
    // 빌드 파일 실행 시 한글 깨짐 방지
    setlocale(LC_ALL, "ko_KR.UTF-8");

    // openai api 연결
    string key;
    cout << "openai api key를 입력하시오: ";
    cin >> key;
    cout << "openai api에 연결합니다" << endl;
    if (!ConnectApi(key)){
        cout << "프로그램을 종료합니다" << endl;
        return 0;
    }

    // 파일 존재 확인
    if (!IsFile()){
        cout << "프로그램을 종료합니다" << endl;
        return 0;
    }

    // 음식 리스트 불러오기
    cout << "음식 목록을 불러옵니다" << endl;
    vector<FoodInfo> food_list = ReadFoodListCsv();

    PrintFunctions();
    while (1){
        // 입력
        int choice; // 사용자 입력을 저장하는 변수
        cout << "사용할 기능을 선택하십시오(기능 보기: 0): ";

        cin >> choice;
        if (cin.fail() || cin.peek() != '\n'){
            cout << "정수를 입력하십시오" << endl;
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
            AddFood(food_list);
        }
        else if (choice == 3){
            DeleteFood(food_list);
        }
        else if (choice == 4){
            PrintExpiringFoods(food_list);
        }
        else if (choice == 5){
            RecommendMenu(food_list);
        }
        else if (choice == 6){
            break;
        }
        else{
            cout << "잘못된 입력입니다" << endl;
        }
    }

    cout << "프로그램을 종료합니다" << endl;
    return 0;
}