#ifndef IOUTILS_HPP
#define IOUTILS_HPP

#include <string>
using namespace std;

wstring InputUtf16(size_t bufferSize); // utf16 문자열 입력받는 함수
string CinName(); // 이름 입력받는 함수
int CinInteger(string message); // 정수 입력받는 함수
int CinDate(); // 날짜 입력받는 함수
int GetOutputLength(const string& str); // 문자열의 출력 길이 계산하는 함수

#endif