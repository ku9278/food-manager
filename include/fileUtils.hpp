#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <vector>
#include "FoodInfo.hpp"
using namespace std;

bool IsFile();
vector<FoodInfo> ReadFoodListCsv();

#endif