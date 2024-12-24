#ifndef FEATURES_HPP
#define FEATURES_HPP

#include <vector>
#include "FoodInfo.hpp"

void PrintFunctions(); // 기능 출력하는 함수
void PrintFoods(const vector<FoodInfo>& list);
void AddFood(vector<FoodInfo>& list, string file_dir);
void DeleteFood(vector<FoodInfo>& list, string file_dir);
void PrintExpiringFoods(const vector<FoodInfo>& list);
void RecommendMenu(vector<FoodInfo>& list);
void ChangeSettings();

#endif