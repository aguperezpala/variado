#include <iostream>
#include <fstream>
#include <sstream>

#include "StreamEvaluator.h"

static const unsigned int MAX_INT = 4096;

static void
printHelp(void)
{
    std::cout << "\n\nYou must call this tool with at least one argument like this:\n"
        << "\ttool <filename> [maxInt]"
            "\n\twhere the filename is the file path with the data to be parsed and\n"
            "\t[maxInt] is an optional value that specifies the size of the numbers"
            "\n\tof ints we want to read at once (as an upper bound). If this value"
            "\n\tis not specified, then 4096 will be used\n\n";
}


int
main(int argc, char** args)
{
    if (argc < 2) {
        printHelp();
        return 1;
    }

    // check if we can open the file
    std::ifstream file(args[1], std::ifstream::in);

    if (!file.is_open()) {
        std::cerr << "Error reading the file " << args[1] << std::endl;
        return 2;
    }

    // check if we will use maxInt
    unsigned int maxInt = MAX_INT;
    if (argc == 3) {
        std::stringstream ss;
        ss << args[2];
        ss >> maxInt;
    }

    // create the StreamEvaluator and evaluate the file
    StreamEvaluator::evaluateStream(file, std::cout, maxInt);

    return 0;
}
