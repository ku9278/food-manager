// 한글 깨짐
// 유통기한 지난 음식 처리 필요
//     1. 파일에서 자동적으로 삭제한다
//         일반적으로 유통기한 하루 이틀 지나도 냉장고에 있는 경우도 있다
//         따라서 냉장고에는 있지만 파일에는 없는 경우가 발생할 수 있다
//     2. 음식 처리는 전적으로 사용자에게 맡긴다
//         유통기한 임박 음식을 출력할 때 이미 지난 음식도 포함시킬지 생각해보아야 한다
//         프로그램을 시작할 때 유통기한이 지난 음식을 알려줄 수 있다.
// 예외처리 더 자세하게
// 잘못 입력했을 때 돌아갈 수 있는 방법이 필요함
// 추천 메뉴 퀄리티가 빈약함
// 함수 정리 필요
// 음식의 이름이 같은 경우(같은 음식이지만 유통기한이 다른 경우) 처리 필요

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
#include <ctime>
#include "openai.hpp"
#include "nlohmann/json.hpp"

#undef max

using namespace std;
using json = nlohmann::json;

/*
csv:
    name1, count1, date1
    name2, count2, date2
    ...
struct:
    name count date
vector:
    struct1
    struct2
    ...
*/

// 음식 정보 구조체
struct FoodInfo{
    string name;
    int count;
    int date; // YYYYMMDD
};

const string file_dir = "food_list.csv";

bool IsFile();
vector<FoodInfo> ReadFoodListCsv();
string CinName(); // 이름 입력받는 함수
int CinCount(); // 개수 입력받는 함수
int CinDate(); // 날짜 입력받는 함수

void PrintFunctions(); // 기능 출력하는 함수
void PrintFoods(const vector<FoodInfo>& list);
void AddFood(vector<FoodInfo>& list);
void DeleteFood(vector<FoodInfo>& list);
void PrintExpiringFoods(const vector<FoodInfo>& list);
void RecommendMenu(vector<FoodInfo>& list);

bool ConnectApi(string key);
string GetResponse(string system_prompt, string query);

int CountDigits(int number); // 자릿수 구하는 함수
int GetCurrentDate(); // 현재 날짜 구하는 함수


int main(){
    // openai api 연결
    string key;
    cout << "openai api key를 입력하시오: ";
    cin >> key;
    if (!ConnectApi(key)){
        cout << "프로그램을 종료합니다" << endl;
        return 0;
    }

    // 파일 존재 확인
    if (!IsFile()){
        cout << "프로그램을 종료합니다" << endl;
        return 0;
    }

    // 음식 리스트 불러오기
    vector<FoodInfo> food_list = ReadFoodListCsv();

    PrintFunctions();
    while (1){
        // 입력
        int choice; // 사용자 입력을 저장하는 변수
        cout << "사용할 기능을 선택하십시오(기능 보기: 0): ";

        cin >> choice;
        if (cin.fail() || cin.peek() != '\n'){
            cout << "정수를 입력하십시오" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 0){
            PrintFunctions();
        }
        else if (choice == 1){
            PrintFoods(food_list);
        }
        else if (choice == 2){
            AddFood(food_list);
        }
        else if (choice == 3){
            DeleteFood(food_list);
        }
        else if (choice == 4){
            PrintExpiringFoods(food_list);
        }
        else if (choice == 5){
            RecommendMenu(food_list);
        }
        else if (choice == 6){
            break;
        }
        else{
            cout << "잘못된 입력입니다" << endl;
        }
    }

    cout << "프로그램을 종료합니다" << endl;
    return 0;
}


bool IsFile(){
    fstream file(file_dir);
    if (!file.is_open()){
        cout << "음식 목록 파일을 찾을 수 없습니다" << endl;
        file.close();
        return false;
    }
    else{
        file.close();
        return true;
    }
}


vector<FoodInfo> ReadFoodListCsv(){
    vector<FoodInfo> food_list;
    FoodInfo food;
    string elem;
    ifstream file(file_dir);

    int i = 0; // while내 인덱스를 위한 변수
    while (file >> elem){
        // ',' 제거
        size_t pos = elem.find(',');
        if (pos != string::npos)
            elem.replace(pos, 1, "");
        // 요소 저장
        switch (i % 3){
            case 0:
                food.name = elem;
                break;
            case 1:
                food.count = stoi(elem);
                break;
            case 2:
                food.date = stoi(elem);
                food_list.push_back(food);
                break;
        }
        i += 1;
    }
    file.close();

    return food_list;
}


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


void PrintFunctions(){
    cout << "1. 현재 음식 보기" << endl;
    cout << "2. 음식 추가" << endl;
    cout << "3. 음식 제거" << endl;
    cout << "4. 유통기한 임박 음식 보기" << endl;
    cout << "5. 추천 메뉴 보기" << endl;
    cout << "6. 프로그램 종료" << endl;
}


void PrintFoods(const vector<FoodInfo>& list){
    int max_len = 0; // 이름 최대 길이
    int max_digits = 0; // 개수 최대 자릿수
    int date_size = 8; // 날짜 길이 (8로 고정)

    for (auto& food : list){
        // 이름 길이
        int len = food.name.length();
        if (len > max_len)
            max_len = len;
        // 개수 자릿수
        int digits = CountDigits(food.count);
        if (digits > max_digits)
            max_digits = digits;
    }

    cout << "|-";
    for (int i = 0; i < max_len; i++) cout << '-';
    cout << "-|-";
    for (int i = 0; i < max_digits; i++) cout << '-';
    cout << "-|-";
    for (int i = 0; i < date_size; i++) cout << '-';
    cout << "-|" << endl;

    for (auto& food : list){
        cout << "| " << food.name;
        for (int i = 0; i < max_len - food.name.length(); i++) cout << ' ';
        cout << " | " << food.count;
        for (int i = 0; i < max_digits - CountDigits(food.count); i++) cout << ' ';
        cout << " | " << food.date << " |" << endl;

        cout << "|-";
        for (int i = 0; i < max_len; i++) cout << '-';
        cout << "-|-";
        for (int i = 0; i < max_digits; i++) cout << '-';
        cout << "-|-";
        for (int i = 0; i < date_size; i++) cout << '-';
        cout << "-|" << endl;
    }
}


void AddFood(vector<FoodInfo>& list){
    string name;
    int count;
    int date;

    // 사용자 입력 받기
    name = CinName();
    count = CinCount();
    date = CinDate();

    // 파일에 입력
    ofstream file(file_dir, ios::app);
    file << '\n' << name << ", " << count << ", " << date;
    file.close();

    // 벡터에 추가
    FoodInfo line;
    line.name = name;
    line.count = count;
    line.date = date;
    list.push_back(line);

    cout << name << " " << count << "개가 추가되었습니다." << endl;
}


void DeleteFood(vector<FoodInfo>& list){
    string name;
    int count;

    // 사용자 입력 받기
    name = CinName();
    count = CinCount();

    // 벡터에서 제거
    vector<FoodInfo>::iterator iter = list.begin();
    vector<FoodInfo>::iterator list_size = list.end();
    for (auto& food : list){
        if (name.compare(food.name) == 0){
            // 입력한 수량이 현재 수량보다 많으면 오류 출력
            if (count > food.count){
                cout << "입력한 수량이 현재 수량을 초과합니다" << endl;
                return;
            }
            else if (count < food.count){
                food.count -= count;
                break;
            }
            else{ // count == food.count
                list.erase(iter);
                break;
            }
        }
        iter += 1;
    }

    // 입력한 음식이 벡터 내에 없으면 오류 출력
    if (iter == list_size){
        cout << "해당 음식이 없습니다" << endl;
        return;
    }

    // 파일에서 제거
    // 제거한 벡터를 파일에 덮어쓰기
    ofstream file(file_dir);
    for (auto food : list){
        file << food.name << ", " << food.count << ", " << food.date << '\n';
    }
    file.close();

    cout << name << " " << count << "개가 제거되었습니다." << endl;
}


void PrintExpiringFoods(const vector<FoodInfo>& list){
    int current_date = GetCurrentDate();
    int threshold = 3; // 유통기한 임박 기준

    // 유통기한 임박 음식 벡터를 생성하여 PrintFood() 함수로 보내준다
    vector<FoodInfo> expiring_food_list;
    for (auto food : list){
        if (food.date - threshold <= current_date)
            expiring_food_list.push_back(food);
    }
    PrintFoods(expiring_food_list);
}


void RecommendMenu(vector<FoodInfo>& list){
    string system_prompt = "영어로 음식 목록을 입력할테니까 이 음식들로 할 수 있는 요리와 간단한 설명만 한글로 출력해.\
                            터미널 환경에서 출력될거야. 적절한 구분선을 같이 출력하고 마크다운 문법은 쓰지마.\
                            밥과 면은 있다고 가정해.";

    // 질문 작성
    // 시스템 프롬프트가 적용되어 있으므로 음식 목록만 전달한다
    string query;
    for (auto food : list){
        query += food.name;
        query += " ";
    }

    // 질문 보내고 대답 받기
    string response = GetResponse(system_prompt, query);

    // 대답 출력
    cout << response << endl;
}


bool ConnectApi(string key){
    // api 연결
    // 인터넷 연결이 없으면 예외를 발생시킨다
    // 잘못된 key를 입력하면 예외를 발생시킨다.
    openai::start(key);
    try{
        auto chat = openai::chat().create(R"(
        {
            "model": "gpt-4o-mini",
            "messages":[{"role":"user", "content":"test"}],
            "max_tokens": 10,
            "temperature": 0
        }
        )"_json);
    }
    catch(runtime_error const& e){
        cerr << e.what() << endl;
        return false;
    }
    return true;
}


string GetResponse(string system_prompt, string query){
    json payload = {
        {"model", "gpt-4o-mini"},
        {"messages", {
            {{"role", "system"}, {"content", system_prompt}},
            {{"role", "user"}, {"content", query}}
        }},
        {"max_tokens", 1000},
        {"temperature", 0}
    };
    json chat = openai::chat().create(payload);
    string response = chat["choices"][0]["message"]["content"];
    return response;
}


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

