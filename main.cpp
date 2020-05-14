#include "lab6.h"

int main() {
    std::cout << "Writing to PNG-file:" << std::endl;
    std::string ver1 = "Hello";
    std::string ver2 = "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    std::string ver3 = ver2 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    std::string ver4 = ver3 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHello"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    std::string ver5 = ver4 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    std::string ver6 = ver5 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    std::string ver7 = ver6 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    std::string ver8 = ver7 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    std::string ver9 = ver8 +
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell"
                       "HelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHelloHell";
    const char* res_char = ver9.c_str();
    Apa::writeBMP(res_char);
    return 0;
}
