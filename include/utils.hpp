#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include<unordered_map>
using namespace std;

int CountDigits(int number); // 자릿수 구하는 함수
int GetCurrentDate(); // 현재 날짜 구하는 함수
string WstringToString(const wstring& wstr);
wstring StringToWstring(const string& str);
string ReplacePlaceHolders(const string& message, const unordered_map<string, string>& replacements);

#endif