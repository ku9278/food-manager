#include <iostream>
#include <string>
#include "openai.hpp"
#include "nlohmann/json.hpp"
#include "language.hpp"
#include "openaiUtils.hpp"
using namespace std;
using json = nlohmann::json;


void ConnectApi(string key){
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
        cout << language_pack["Exit the program"];
        exit(1);
    }
}


string GetResponse(string system_prompt, string query){
    json payload = {
        {"model", "gpt-4o-mini"},
        {"messages", {
            {{"role", "system"}, {"content", system_prompt}},
            {{"role", "user"}, {"content", query}}
        }},
        {"max_tokens", 1000},
        {"temperature", 1}
    };
    json chat = openai::chat().create(payload);
    string response = chat["choices"][0]["message"]["content"];
    return response;
}