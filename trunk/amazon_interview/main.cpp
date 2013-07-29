#include <iostream>
#include <fstream>

#include "Parser.h"


void
printStatus(const std::ifstream& s)
{
    std::cout << "good(): " << s.good() << std::endl;
    std::cout << "eof(): " << s.eof() << std::endl;
    std::cout << "bad(): " << s.bad() << std::endl;
}


int
main(int argc, char** args)
{
    std::ifstream file("test.txt", std::ifstream::in);
    Parser parser(file);

    file >> std::ws;
    printStatus(file);
    for (int i = 0; i < 3; ++i) {
        std::cout << "char: " << (char)file.get() << std::endl;
    }
    printStatus(file);

    /*
    int x = 0, y = 0;

    char c;
    if (std::cin >> x  >> c >> y) {
        std::cout << "everything ok:" << x << " , " << y << "\n";
    } else {
        std::cout << "Error\n";
    }*/
    return 0;
}
