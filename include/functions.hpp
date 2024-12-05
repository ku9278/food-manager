#ifndef FUNSTIONS_HPP
#define FUNSTIONS_HPP

#include <vector>
#include "FoodInfo.hpp"

void PrintFunctions(); // 기능 출력하는 함수
void PrintFoods(const vector<FoodInfo>& list);
void AddFood(vector<FoodInfo>& list);
void DeleteFood(vector<FoodInfo>& list);
void PrintExpiringFoods(const vector<FoodInfo>& list);
void RecommendMenu(vector<FoodInfo>& list);
void ChangeSettings();

#endif