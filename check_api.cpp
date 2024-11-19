#include <iostream>
#include "openai.hpp"

int main() {
    std::string key;
    std::cout << "key: ";
    std::cin >> key;
    openai::start(key);

    auto chat = openai::chat().create(R"(
    {
        "model": "gpt-3.5-turbo",
        "messages":[{"role":"user", "content":"달걀로 할 수 있는 요리 리스트"}],
        "max_tokens": 100,
        "temperature": 0
    }
    )"_json);
    std::cout << "Response is:\n" << chat.dump(2) << '\n'; 
}