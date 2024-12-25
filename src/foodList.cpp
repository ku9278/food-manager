#include <vector>
#include <fstream>
#include "fileUtils.hpp"
#include "FoodInfo.hpp"

vector<FoodInfo> food_list;

void ReadFoodList(){
    ifstream file(food_list_dir, ios::binary);

    string name;
    int count;
    int date;
    while (file >> name >> count >> date){
        FoodInfo food = {name, count, date};
        food_list.push_back(food);
    }

    file.close();
}