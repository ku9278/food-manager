#include <iostream>
#include <fstream>
#include <vector>
#include "nlohmann/json.hpp"
#include "FoodInfo.hpp"
#include "fileUtils.hpp"
using namespace std;
using json = nlohmann::json;


// IsFile() 함수는 영어로만 출력
void IsFile(string file_dir){
    fstream file(file_dir);
    if (!file.is_open()){
        string file_name;
        size_t pos = file_dir.find_last_of("/");
        if (pos == string::npos)
            file_name = file_dir;
        else
            file_name = file_dir.substr(pos + 1);
        
        cout << file_name << " not found" << endl;
        cout << "Exit the program" << endl;
        file.close();
        exit(1);
    }
    file.close();
}


vector<FoodInfo> ReadFoodList(string file_dir){
    vector<FoodInfo> food_list;
    ifstream file(file_dir, ios::binary);

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