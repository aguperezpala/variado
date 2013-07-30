#include <iostream>
#include <fstream>
#include <sstream>


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


    std::stringstream ss(" 0, 1, 2, 3, 4 , 5 , 6 , 7 , 8 , 9\n");
    ss >> std::noskipws;
    int i;
    while (ss >> i) {
        std::cout << "value: " << i << std::endl;
        ss.ignore();
    }

    int x1,x2,x3;
    if (!(ss >> x1)) {
        std::cout << "x1 fail\n";
    } else {
        std::cout << "x1 ok:" << x1 << "\n";
    }
    char c;
    ss >> c;
    if (!(ss >> x2)) {
        std::cout << "x2 fail\n";
    } else {
        std::cout << "x2 ok: " << x2 << "\n";
    }


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
