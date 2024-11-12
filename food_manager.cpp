// 한글 깨짐
// 입력한 유통기한이 현재 날짜를 지났는지 확인 필요 (유통기한 임박 음식 출력 기능 추가할 때 구현)

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

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

bool isFile();
string cinName(); // 이름 입력받는 함수
int cinCount(); // 개수 입력받는 함수
int cinDate(); // 날짜 입력받는 함수

void printFunction(); // 기능 출력하는 함수
void printFood(vector<FoodInfo> list);
void addFood(vector<FoodInfo>& list);
void deleteFood(vector<FoodInfo>& list);

int countDigits(int number); // 자릿수 구하는 함수

int main(){
    // 음식 리스트 불러오기
    if (!isFile()){
        cout << "프로그램을 종료합니다" << endl;
        return 0;
    }

    vector<FoodInfo> food_list; // 음식 리스트를 저장하는 벡터
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

    printFunction();
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
            printFunction();
        }
        else if (choice == 1){
            printFood(food_list);
        }
        else if (choice == 2){
            addFood(food_list);
        }
        else if (choice == 3){
            deleteFood(food_list);
        }
        else if (choice == 4){

        }
        else if (choice == 5){

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


bool isFile(){
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


string cinName(){
    string name;
    cout << "음식 이름: ";
    cin >> name;
    return name;
}


int cinCount(){
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


int cinDate(){
    int date;

    while(1){
        cout << "유통기한: ";
        cin >> date;
        if (cin.fail() || cin.peek() != '\n'){
            cout << "날짜 포맷에 맞춰 입력하십시오(YYYYMMDD)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (countDigits(date) != 8){
            cout << "날짜 포맷에 맞춰 입력하십시오(YYYYMMDD)" << endl;
            continue;
        }
        break;
    }

    return date;
}


void printFunction(){
    cout << "1. 현재 음식 보기" << endl;
    cout << "2. 음식 추가" << endl;
    cout << "3. 음식 제거" << endl;
    cout << "4. 유통기한 임박 음식 보기" << endl;
    cout << "5. 추천 메뉴 보기" << endl;
    cout << "6. 프로그램 종료" << endl;
}


void printFood(vector<FoodInfo> list){
    int max_len = 0; // 이름 최대 길이
    int max_digits = 0; // 개수 최대 자릿수
    int date_size = 8; // 날짜 길이 (8로 고정)

    for (auto food : list){
        // 이름 길이
        int len = food.name.length();
        if (len > max_len)
            max_len = len;
        // 개수 자릿수
        int digits = countDigits(food.count);
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

    for (auto food : list){
        cout << "| " << food.name;
        for (int i = 0; i < max_len - food.name.length(); i++) cout << ' ';
        cout << " | " << food.count;
        for (int i = 0; i < max_digits - countDigits(food.count); i++) cout << ' ';
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


void addFood(vector<FoodInfo>& list){
    string name;
    int count;
    int date;

    // 사용자 입력 받기
    name = cinName();
    count = cinCount();
    date = cinDate();

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


void deleteFood(vector<FoodInfo>& list){
    string name;
    int count;

    // 사용자 입력 받기
    name = cinName();
    count = cinCount();

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


int countDigits(int number){
    int count = 0;
    while (number != 0) {
        number /= 10;
        count++;
    }
    return count;
}

