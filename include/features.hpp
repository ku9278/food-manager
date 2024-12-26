#ifndef FEATURES_HPP
#define FEATURES_HPP

#include <vector>
#include "FoodInfo.hpp"

void PrintFunctions(); // 기능 출력하는 함수
void PrintFoods(const vector<FoodInfo>& list);
void AddFood();
void DeleteFood();
void PrintExpiringFoods();
void RecommendMenu();
void ChangeSettings();

#endif