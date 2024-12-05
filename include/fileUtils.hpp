#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <vector>
#include "FoodInfo.hpp"
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

bool IsFile();
vector<FoodInfo> ReadFoodListCsv();
json ReadJson(string dir);
void WriteJson(string dir, json js);

#endif