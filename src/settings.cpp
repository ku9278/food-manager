#include <iostream>
#include <fstream>
#include "fileUtils.hpp"
#include "foodList.hpp"
#include "language.hpp"
#include "settings.hpp"

void Initialize(){
    // 초기화 여부 확인
    char confirmation;
    cout << language_pack["Settings and food lists will be initialized"] << endl;
    cout << language_pack["Are you sure you want to initialize the program? [y/n]"] << endl;
    cout << language_pack["Select: "];
    cin >> confirmation;
    if (confirmation == 'n') return;

    // 음식 목록 파일 초기화
    ofstream file(food_list_dir, ios::trunc | ios::binary);
    file << "";
    file.close();
    
    // 음식 목록 벡터 초기화
    food_list.clear();

    // 설정 초기화
    // 초기화 할 설정 없음

    cout << language_pack["Initialization completed"] << endl;
}