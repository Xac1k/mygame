#include <string>
#include <math.h>
#include <vector>
#include <iostream>

#define chFilling "=";
#define LenForAligin 70;

int main() {
    // std::cout << "Введети желаемое количество символов в строке\n";
    int len = LenForAligin;
    // std::cin >> len;
    std::cout << "Введите слово, которое хотите отцентровать\n";
    std::string mainWord;
    std::cin >> mainWord;
    // std::cout << "Введите символ заполнения \n";
    std::string chFill = chFilling;
    // std::cin >> chFill;

    int lenFilling = len - mainWord.size();
    int lenFillingBeforeWord = lenFilling / 2;
    std::string res;

    for(int i = 0; i < lenFillingBeforeWord; i++) {
        res += chFill;
    }
    res += mainWord;
    for(int i = 0; i < lenFilling - lenFillingBeforeWord; i++) {
        res += chFill;
    }

    std::cout << res << std::endl;
    return 0;
}