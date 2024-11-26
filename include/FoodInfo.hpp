#ifndef FOODINFO_HPP
#define FOODINFO_HPP

#include <string>
using namespace std;

// 음식 정보 구조체
struct FoodInfo{
    string name;
    int count;
    int date; // YYYYMMDD
};

#endif