#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <UnitTest++/UnitTest++.h>

#include "Parser.h"
#include "Debug.h"


////////////////////////////////////////////////////////////////////////////////
//                              Parser tests                                  //
////////////////////////////////////////////////////////////////////////////////

TEST(ParserFunctionNameEmpty)
{
    std::string funName;

    // check for empty stream
    {
        std::stringstream ss("");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

    // check for an empty line without function name
    {
        std::stringstream ss("\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::EndOfLine, parser.parseFunction(funName));
        CHECK_EQUAL(0, funName.size());

        // then we should get an EndOfFile
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }


}

TEST(ParserIllFormedFunctionNames)
{
    std::string funName;

    // check for a function without : character
    {
        std::stringstream ss("pep");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

    // check for a function with 1 letter and a \n
    {
        std::stringstream ss("1:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

    // check for a function with 3 letters and \n
    {
        std::stringstream ss("sum\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

    // check for a function with 2 letters and :
    {
        std::stringstream ss("me:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
    }
    {
        std::stringstream ss("me:");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
    }

    // check for a function with 2 characters and a lot of empty spaces and then
    // another character and the :
    {
        std::stringstream ss("su    \t\t\t\t\t\t\t    m:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

}

TEST(ParserEmptySpacesFunctionNames)
{
    std::string funName;

    // check for a function with a lot of empty characters at the beginning
    {
        std::stringstream ss("\t\t\t\t\t\r   \tsum:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName));
        CHECK_EQUAL("sum", funName);
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

    // check for a function with a lot of empty characters after the name and
    // before the :
    {
        std::stringstream ss("\t\t\t\t\t\r   \tsum\t\t\t\t\t  \t\t\t:\n");
        Parser parser(ss);
        funName = "";
        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName));
        CHECK_EQUAL("sum", funName);
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }
}

TEST(ParserWrongFunctionNamesSkippingLine)
{
    std::string funName;

    // check for an illformed name and a lot of characters after that
    {
        std::stringstream ss("sum  aasdasda sd as as das as :\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }
    {
        std::stringstream ss("su\ts:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

}

TEST(ParserMultiFunctionNames)
{
    // check for a 2 correct function names
    {
        std::string funName, funName2;
        std::stringstream ss("sum:\nmin:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName2));
        CHECK_EQUAL("sum", funName);
        CHECK_EQUAL("min", funName2);
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }

    // check for a correct -> incorrect -> correct function name
    {
        std::string funName, funName2, funName3;
        std::stringstream ss("sum:\nmi n:\navg:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseFunction(funName2));
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName3));
        CHECK_EQUAL("sum", funName);
        CHECK_EQUAL("avg", funName3);
        parser.skipCurrentLine();
        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseFunction(funName));
    }
}

TEST(ParserStreamPositionMethods)
{
    // read a correct function and then skip all. then read it again
    {
        std::string funName, funName2;
        std::stringstream ss("sum:\n");
        Parser parser(ss);

        const size_t beg = parser.streamPosition();

        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName));
        parser.skipCurrentLine();
        parser.setStreamPosition(beg);
        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName2));
        CHECK_EQUAL("sum", funName);
        CHECK_EQUAL("sum", funName2);
    }

    // read a function, skip everything, read the second one and then go back
    // to the second one and read it again
    {
        std::string funName, funName2, funName3;
        std::stringstream ss("sum:\nmin:\n");
        Parser parser(ss);

        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName));
        parser.skipCurrentLine();
        const size_t second = parser.streamPosition();
        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName2));
        parser.skipCurrentLine();
        parser.setStreamPosition(second);
        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseFunction(funName3));
        CHECK_EQUAL("sum", funName);
        CHECK_EQUAL("min", funName2);
        CHECK_EQUAL("min", funName3);
    }
}

//
// Parsing numbers now
//

TEST(ParserInvalidNumbers)
{
    std::vector<int> numbers;

    // check for empty numbers
    {
        std::stringstream ss("");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::EndOfFile, parser.parseNumbers(10, numbers));
        CHECK_EQUAL(0, numbers.size());

    }
    {
        std::stringstream ss("\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::EndOfLine, parser.parseNumbers(10, numbers));
        CHECK_EQUAL(0, numbers.size());

    }

    // check for wrong numbers ending
    {
        std::stringstream ss("1");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseNumbers(10, numbers));
    }

    // check for some correct and then some invalid numbers
    {
        std::stringstream ss("1,2,asd\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseNumbers(10, numbers));
    }
    {
        std::stringstream ss("1,2.2\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseNumbers(10, numbers));
    }

    // check for wrong separation
    {
        std::stringstream ss("1,2;3\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseNumbers(10, numbers));
    }
    {
        std::stringstream ss("     ,1,2\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseNumbers(10, numbers));
    }
    {
        std::stringstream ss("1,2,,3\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::InvalidFormat, parser.parseNumbers(10, numbers));
    }

}

TEST(ParserValidNumberShortList)
{
    std::vector<int> numbers;

    // parse for 0 numbers in a valid list
    {
        std::stringstream ss("0,1,2,3, 4, 5, 6, 7 ,8 ,9\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseNumbers(0, numbers));
        CHECK_EQUAL(0, numbers.size());
    }

    // check for some correct numbers
    {
        std::stringstream ss("0, 1, 2, 3, 4, 5, 6, 7, 8, 9\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::EndOfLine, parser.parseNumbers(10, numbers));
        CHECK_EQUAL(10, numbers.size());
        for (size_t i = 0, size = numbers.size(); i < size; ++i) {
            CHECK_EQUAL(i, numbers[i]);
        }
    }

    // read more numbers we have
    {
        std::stringstream ss("0,1,2,3, 4, 5, 6, 7 ,8 ,9\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::EndOfLine, parser.parseNumbers(15, numbers));
        CHECK_EQUAL(10, numbers.size());
        for (size_t i = 0, size = numbers.size(); i < size; ++i) {
            CHECK_EQUAL(i, numbers[i]);
        }
    }

    // check for numbers with a lot of different spaces between them
    {
        std::stringstream ss("\t\t\t\t    0   \t, \t\t 1 \t , \t  2 , \t3, "
            " 4\t\t, \t 5,  \t6\t,        7       ,\t8\t\t\t ,\t\t\t\t9\t\t\t\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::EndOfLine, parser.parseNumbers(10, numbers));
        CHECK_EQUAL(10, numbers.size());
        for (size_t i = 0, size = numbers.size(); i < size; ++i) {
            CHECK_EQUAL(i, numbers[i]);
        }
    }

    // read for the list into 3 times
    {
        std::stringstream ss("0,1,2,3,4,5,6,7,8,9,10,11,12,13,14\n");
        Parser parser(ss);
        numbers.clear();

        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseNumbers(5, numbers));
        CHECK_EQUAL(5, numbers.size());
        size_t startIndex = 0;
        for (size_t size = numbers.size(); startIndex < size; ++startIndex) {
            CHECK_EQUAL(startIndex, numbers[startIndex]);
        }

        // second read
        CHECK_EQUAL(Parser::ResultCode::NoError, parser.parseNumbers(5, numbers));
        CHECK_EQUAL(5, numbers.size());

        for (size_t i = 0, size = numbers.size(); i < size; ++i) {
            CHECK_EQUAL(startIndex + i, numbers[i]);
        }

        // third read
        CHECK_EQUAL(Parser::ResultCode::EndOfLine, parser.parseNumbers(5, numbers));
        CHECK_EQUAL(5, numbers.size());
        startIndex += 5;


        for (size_t i = 0, size = numbers.size(); i < size; ++i) {
            CHECK_EQUAL(startIndex + i, numbers[i]);
        }
    }

}

TEST(ParserValidNumbersLongList)
{
    // check for a lot of numbers

    // check for multiple read (read less numbers than the real list)

}



int
main(void)
{
    return UnitTest::RunAllTests();
}
