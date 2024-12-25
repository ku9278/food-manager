#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <vector>
#include "FoodInfo.hpp"
#include "nlohmann/json.hpp"
using namespace std;
using json = nlohmann::json;

// 파일 경로
const string food_list_dir = "food_list.bin";
const string settings_dir = "settings.json";

void IsFile(string file_dir);
json ReadJson(string dir);
void WriteJson(string dir, json js);

#endif