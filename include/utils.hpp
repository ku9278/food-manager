#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
using namespace std;

int CountDigits(int number); // 자릿수 구하는 함수
int GetCurrentDate(); // 현재 날짜 구하는 함수
string WstringToString(const wstring& wstr);
wstring StringToWstring(const string& str);

#endif