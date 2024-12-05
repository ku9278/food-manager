#ifndef LANGUAGE_HPP
#define LANGUAGE_HPP

#include <unordered_map>
#include <string>
using namespace std;

extern unordered_map<string, string> language_pack;

void LoadLanguagePack(string language);
void SetLanguage();

#endif