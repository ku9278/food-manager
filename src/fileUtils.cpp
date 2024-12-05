#include <iostream>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"
#include "FoodInfo.hpp"
#include "language.hpp"
#include "constants.hpp"
#include "fileUtils.hpp"
using namespace std;
using json = nlohmann::json;


bool IsFile(){
    fstream file(food_list_dir);
    if (!file.is_open()){
        cout << language_pack["file_not_found"] << endl;
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
    ifstream file(food_list_dir);

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


json ReadJson(string dir){
    ifstream json_file(dir);
    json js = json::parse(json_file);
    return js;
}


void WriteJson(string dir, json js){
    ofstream json_file(dir);
    json_file << js;
}