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


vector<FoodInfo> ReadFoodList(){
    vector<FoodInfo> food_list;
    ifstream file(food_list_dir, ios::binary);

    string name;
    int count;
    int date;
    while (file >> name >> count >> date){
        FoodInfo food = {name, count, date};
        food_list.push_back(food);
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