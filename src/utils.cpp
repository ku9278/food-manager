#include <ctime>
#include <utils.hpp>
#include <locale>
#include <codecvt>


int CountDigits(int number){
    int count = 0;
    while (number != 0) {
        number /= 10;
        count++;
    }
    return count;
}


int GetCurrentDate(){
    int current_date;

    time_t raw_time;
    struct tm* time_info;
    raw_time = time(NULL);
    time_info = localtime(&raw_time);

    int current_y = time_info->tm_year + 1900;
    int current_m = time_info->tm_mon + 1;
    int current_d = time_info->tm_mday;

    current_date = current_y*10000 + current_m*100 + current_d;
    return current_date;
}


string WstringToString(const wstring& wstr){
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}


wstring StringToWstring(const string& str){
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}