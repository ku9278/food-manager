#ifndef OPENAIUTILS_HPP
#define OPENAIUTILS_HPP

#include <string>
using namespace std;

bool ConnectApi(string key);
string GetResponse(string system_prompt, string query);

#endif