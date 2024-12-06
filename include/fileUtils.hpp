#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <vector>
#include "FoodInfo.hpp"
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

// bool IsFile(string file_dir);
void IsFile(string file_dir);
vector<FoodInfo> ReadFoodList(string file_dir);
json ReadJson(string dir);
void WriteJson(string dir, json js);

#endif