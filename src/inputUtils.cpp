#include <iostream>
#include <string>
#include <limits>
#include "utils.hpp"
#include "inputUtils.hpp"
#undef max
using namespace std;


string CinName(){
    string name;
    cout << "음식 이름: ";
    cin >> name;
    return name;
}


int CinCount(){
    int count;

    while(1){
        cout << "개수: ";
        cin >> count;
        if (cin.fail() || cin.peek() != '\n'){
            cout << "정수를 입력하십시오" << endl;
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
        cout << "유통기한: ";
        cin >> date;
        if (cin.fail() || cin.peek() != '\n'){
            cout << "날짜 포맷에 맞춰 입력하십시오(YYYYMMDD)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (CountDigits(date) != 8){
            cout << "날짜 포맷에 맞춰 입력하십시오(YYYYMMDD)" << endl;
            continue;
        }
        if (date < current_date){
            cout << "유통기한이 이미 지났습니다" << endl;
            continue;
        }
        break;
    }

    return date;
}