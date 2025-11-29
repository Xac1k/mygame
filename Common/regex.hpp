#pragma once
#include <string>
#include <cctype>

//name*.name, где * любой символ
inline bool regexmatch(std::string pattern, std::string target) {
    for (char& c : pattern) c = std::tolower(static_cast<unsigned char>(c));
    for (char& c : target) c = std::tolower(static_cast<unsigned char>(c));

    int patternID = 0;
    for(auto ch : target) {
        if(patternID + 1 < pattern.size() && pattern[patternID+1] == ch && pattern[patternID] == '*') patternID++;
        if(pattern[patternID] == ch) patternID ++;
        else {
            if(pattern[patternID] == '*') {
                continue;
            }
            else return false;
        }
    }

    int len = pattern.length() - 1;
    return patternID == len;
}