#include <iostream>
#include <fstream>
#include <vector>
#include "FoodInfo.hpp"
#include "constants.hpp"
#include "fileUtils.hpp"
using namespace std;


bool IsFile(){
    fstream file(file_dir);
    if (!file.is_open()){
        cout << "음식 목록 파일을 찾을 수 없습니다" << endl;
        file.close();
        return false;
    }
    else{
        file.close();
        return true;
    }
}


vector<FoodInfo> ReadFoodListCsv(){
    vector<FoodInfo> food_list;
    FoodInfo food;
    string elem;
    ifstream file(file_dir);

    int i = 0; // while내 인덱스를 위한 변수
    while (file >> elem){
        // ',' 제거
        size_t pos = elem.find(',');
        if (pos != string::npos)
            elem.replace(pos, 1, "");
        // 요소 저장
        switch (i % 3){
            case 0:
                food.name = elem;
                break;
            case 1:
                food.count = stoi(elem);
                break;
            case 2:
                food.date = stoi(elem);
                food_list.push_back(food);
                break;
        }
        i += 1;
    }
    file.close();

    return food_list;
}