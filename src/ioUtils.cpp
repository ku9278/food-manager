#include <iostream>
#include <string>
#include <limits>
#include <windows.h>
#include "language.hpp"
#include "utils.hpp"
#include "ioUtils.hpp"
#undef max
using namespace std;


wstring InputUtf16(size_t bufferSize) {
    wstring buffer(bufferSize, L'\0'); // 버퍼
    DWORD char_num = 0; // 읽은 문자 수

    HANDLE stdinHandle = GetStdHandle(STD_INPUT_HANDLE); // 콘솔 입력 핸들
    if (stdinHandle == INVALID_HANDLE_VALUE) {
        return L"";
    }

    ReadConsoleW(stdinHandle, buffer.data(), bufferSize - 1, &char_num, NULL); // 콘솔 입력 읽기
    buffer.resize(char_num); // 읽은 문자 수만큼 크기 조정

    // '\r' 또는 '\n' 제거
    if (char_num > 0 && buffer[char_num - 1] == L'\n') {
        buffer.resize(char_num - 1);
        char_num -= 1;
    }
    if (char_num > 0 && buffer[char_num - 1] == L'\r') {
        buffer.resize(char_num - 1);
        char_num -= 1;
    }

    return buffer;
}


string CinName(){
    wstring name_w;
    string name;
    cout << language_pack["food name: "];
    name_w = InputUtf16(256);
    name = WstringToString(name_w);
    return name;
}


int CinCount(){
    int count;

    while(1){
        cout << language_pack["count: "];
        cin >> count;
        if (cin.fail() || cin.peek() != '\n'){
            cout << language_pack["Please enter an integer"] << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        break;
    }

    return count;
}


int CinDate(){
    int date;
    int current_date = GetCurrentDate();

    while(1){
        cout << language_pack["expiration date (YYYYMMDD): "];
        cin >> date;
        if (cin.fail() || cin.peek() != '\n'){
            cout << language_pack["Please enter the date in the specified format (YYYYMMDD)"] << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (CountDigits(date) != 8){
            cout << language_pack["Please enter the date in the specified format (YYYYMMDD)"] << endl;
            continue;
        }
        if (date < current_date){
            cout << language_pack["The expiration date has passed"] << endl;
            continue;
        }
        break;
    }

    return date;
}


/*
string에 한글과 영어가 섞여서 저장되어있다고 하면
한글개수: a
영어개수: b
라고 할 때
string.length(): 3a + b
string을 wstring으로 변환 후 wstring.length(): a + b

콘솔에서 출력될 때 한글은 2칸을 차지하고 영어는 1칸을 차지하므로 2a + b가 필요하다.
2a + b를 구하기 위해 
2a + b
= (4a + 2b) / 2
= ((3a + b) + (a + b)) / 2
= (string.length() + wstring.length()) / 2
를 이용한다.

이 방식은 한글, 일본어, 중국어와 같은 문자들이 3바이트를 차지하기 때문에 가능하다.
유니코드는 1 ~ 4 바이트를 사용한다.
즉, 모든 유니코드에 대해서 성립하지 않는다.
하지만 프로그램에서 지원할 언어는 모두 3바이트이다.
따라서 3바이트 이외의 입력은 처리하지 않는다.

더 많은 언어나 특수문자 지원이 필요할 경우 유니코드 범위를 나눠서 하는 방식을 생각해본다.
*/
int GetOutputLength(const string& str){
    wstring wstr = StringToWstring(str);
    return (str.length() + wstr.length()) / 2;
}