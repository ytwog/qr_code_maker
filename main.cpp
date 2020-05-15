#include "lab6.h"

int main() {
    std::cout << "Writing to PNG-file:" << std::endl;
    std::string num1 = "21523656";
    std::string num2 = "21523656443264647343464575453253653357348";

    std::string ver1 = "Hello";
    std::string ver2 = "HelloHelloHelloHelloHelloHelloHelloHello";
    std::string ver3 = ver2 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloH";
    std::string ver4 = ver3 +
                       "HelloHelloHelloHelloHe";
    std::string ver5 = ver4 +
                       "HelloHelloHelloHelloHelloHelloHell";
    std::string ver6 = ver5 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHe";
    std::string ver7 = ver6 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloH";
    std::string ver8 = ver7 +
                       "HelloHelloHelloHelloHelloHelloHelloH";
    std::string ver9 = ver8 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHellHelloHell"
                       "oHelloHelloHelloHelloHelloHelloHell";
    const char* res_char = num1.c_str();
    Apa::writeBMP(res_char);
    return 0;
}
