#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <UnitTest++/UnitTest++.h>

#include "Parser.h"
#include "Defines.h"
#include "Debug.h"
#include "StreamFunction.h"
#include "FunctionBuilder.h"
#include "StreamEvaluator.h"



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
    std::vector<IntegerType> numbers;

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
    std::vector<IntegerType> numbers;

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
    // TODO
    // check for a lot of numbers

    // check for multiple read (read less numbers than the real list)

}

////////////////////////////////////////////////////////////////////////////////
//                          StreamFunctions tests                             //
////////////////////////////////////////////////////////////////////////////////

// define a static FunctionBuilder instance....
//
static FunctionBuilder fBuilder;


TEST(StreamFunction_Sum)
{
    StreamFunction* sum = fBuilder.getFunction("sum");
    CHECK_EQUAL(sum, fBuilder.getFunction("Sum"));
    CHECK_EQUAL(sum, fBuilder.getFunction("SUm"));
    CHECK_EQUAL(sum, fBuilder.getFunction("SUM"));

    std::vector<IntegerType> input;
    input.reserve(100);

    for (IntegerType i = 1; i <= 100; ++i) {
        input.push_back(i);
    }
    const IntegerType resultSummatory = (100 * (100 + 1)) / 2;

    // calculate values from a simple vector
    sum->resetForNewData();
    sum->pushInputValues(input);

    {
        std::stringstream ss;
        ss << resultSummatory;
        CHECK_EQUAL(ss.str(), sum->simEvaluation());
    }


    // calculate values from multiple inputs
    sum->resetForNewData();
    sum->pushInputValues(input);
    sum->pushInputValues(input);
    sum->pushInputValues(input);

    {
        std::stringstream ss;
        ss << (resultSummatory * 3);
        CHECK_EQUAL(ss.str(), sum->simEvaluation());
    }
}

TEST(StreamFunction_Min)
{
    StreamFunction* fun = fBuilder.getFunction("min");
    CHECK_EQUAL(fun, fBuilder.getFunction("Min"));
    CHECK_EQUAL(fun, fBuilder.getFunction("MIn"));
    CHECK_EQUAL(fun, fBuilder.getFunction("mIN"));

    std::vector<IntegerType> input;
    input.reserve(120);

    for (IntegerType i = -10; i <= 100; ++i) {
        input.push_back(i);
    }

    // calculate values from a simple vector
    fun->resetForNewData();
    fun->pushInputValues(input);

    CHECK_EQUAL("-10", fun->simEvaluation());


    // calculate values from multiple inputs
    fun->resetForNewData();
    fun->pushInputValues(input);
    for (IntegerType i = 0, size = input.size(); i < size; ++i) {
        input.push_back(input[i] - 100);
    }
    fun->pushInputValues(input);
    fun->pushInputValues(input);

    CHECK_EQUAL("-110", fun->simEvaluation());
}

TEST(StreamFunction_Avg)
{
    // TODO: the checks for the double cases could fail if we are trying to check
    // the values directly, for this simple cases there are a low probabilities
    // that we can get a fail
    //

    StreamFunction* fun = fBuilder.getFunction("avg");
    CHECK_EQUAL(fun, fBuilder.getFunction("Avg"));
    CHECK_EQUAL(fun, fBuilder.getFunction("AVG"));
    CHECK_EQUAL(fun, fBuilder.getFunction("aVG"));

    std::vector<IntegerType> input;
    input.reserve(100);

    for (IntegerType i = 1; i <= 100; ++i) {
        input.push_back(i);
    }
    const IntegerType resultSummatory = (100 * (100 + 1)) / 2;
    const DoubleType avgSummatory = static_cast<DoubleType>(resultSummatory) /
        static_cast<DoubleType>(input.size());

    // calculate values from a simple vector
    fun->resetForNewData();
    fun->pushInputValues(input);

    {
        std::stringstream ss;
        ss << avgSummatory;
        CHECK_EQUAL(ss.str(), fun->simEvaluation());
    }


    // calculate values from multiple inputs
    fun->resetForNewData();
    fun->pushInputValues(input);
    fun->pushInputValues(input);

    {
        std::stringstream ss;
        ss << avgSummatory;
        CHECK_EQUAL(ss.str(), fun->simEvaluation());
    }

    // add the double of the input one more time
    fun->resetForNewData();
    fun->pushInputValues(input);
    for (size_t i = 0, size = input.size(); i < size; ++i) {
        input[i] *= 2;
    }
    fun->pushInputValues(input);
    {
        std::stringstream ss;
        ss << avgSummatory * 1.5;
        CHECK_EQUAL(ss.str(), fun->simEvaluation());
    }
}

////////////////////////////////////////////////////////////////////////////////
//                        Stream Evaluator Tests                              //
////////////////////////////////////////////////////////////////////////////////

TEST(StreamEvaluator_SimpleCorrectValues)
{
    // test empty stream
    {
        std::stringstream in(""), out("");
        StreamEvaluator::evaluateStream(in, out, 10000);
        CHECK_EQUAL(out.str(), "");
    }

    // test one line stream
    {
        std::stringstream in("\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 10000);
        std::string result(StreamEvaluator::EMPTY_LINE);
        result = result + "\n";
        CHECK_EQUAL(result, out.str());
    }

    // test single line with correct values
    {
        std::stringstream in("min:999,88,55,33,22,1,86\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 10000);
        CHECK_EQUAL("1\n", out.str());
    }

    // test single long line (that have to be readed multiple times)
    {
        std::stringstream in("   min:   \t\t999, \t\t\t88,55,     33, 22,1,  86\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        CHECK_EQUAL("1\n", out.str());
    }

    // test one correct line, 2 empty, another correct.
    {
        std::stringstream in("\t\t\t\t     min:999,88,55,33,22,1,86\n\n\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result("1\n");
        result = result + StreamEvaluator::EMPTY_LINE + "\n" +
            StreamEvaluator::EMPTY_LINE + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }
}

TEST(StreamEvaluator_SimpleIncorrectValues)
{
    // test single line with illformed function + correct
    {
        std::stringstream in("misn:999,88,55,33,22,1,86\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result(StreamEvaluator::INVALID_LINE_FORMAT);
        result = result + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }
    {
        std::stringstream in("min \t\t\t999,88,55,33,22,1,86\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result(StreamEvaluator::INVALID_LINE_FORMAT);
        result = result + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }
    {
        std::stringstream in("mi: \t\t\t999,88,55,33,22,1,86\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result(StreamEvaluator::INVALID_LINE_FORMAT);
        result = result + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }

    // test single line with illformed numbers + correct
    {
        std::stringstream in("min: \t\t\t999,8a8,55,33,22,1,86\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result(StreamEvaluator::INVALID_LINE_FORMAT);
        result = result + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }
    {
        std::stringstream in("min: \t\t\t999,8 8,55,33,22,1,86\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result(StreamEvaluator::INVALID_LINE_FORMAT);
        result = result + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }
    {
        std::stringstream in("min: \t\t\t999,8 8,55,3*3,22,1,86\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result(StreamEvaluator::INVALID_LINE_FORMAT);
        result = result + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }

    // test single line with unknown function + correct
    {
        std::stringstream in("mln: \t\t\t999,8 8,55,3*3,22,1,86\nsum: 1, 2, 2\n"), out("");
        StreamEvaluator::evaluateStream(in, out, 2);
        std::string result(StreamEvaluator::UNKNOWN_FUNCTION_NAME);
        result = result + "\n" + "5\n";
        CHECK_EQUAL(result, out.str());
    }

}

TEST(StreamEvaluator_BigData)
{
    // do the big test here contemplating all the possible cases at once
    std::vector<IntegerType> integers;
    integers.reserve(100);

    // start pushing the data into input stream and the expected output result
    std::stringstream in(""), expected("");

    // min = -4
    in << " min\t\t:\t    \t";
    for (IntegerType i = 0; i < 100; ++i) {
        integers.push_back(i - 4);
        in << integers[i] << "   \t\t   ,  \t  ";
    }
    in << "51\n";
    expected << "-4\n"; // expected result for this operation

    // put a invalid function now
    in << "\t\t\r\rwrong_func: 1,2,3,4,5\n";
    expected << StreamEvaluator::INVALID_LINE_FORMAT << "\n";

    // put a empty line
    in << "\n";
    expected << StreamEvaluator::EMPTY_LINE << "\n";

    // put a sum that should value
    IntegerType accum = 0;
    in << "\t\tsum\t\t\t\t  :";
    for (size_t i = 0; i < 100; ++i) {
        accum += integers[i];
        in << integers[i] << "   \t\t   ,  \t  ";
    }
    in << "0\n";
    expected << accum << "\n";

    // avg
    in << "AVG   :";
    DoubleType avg = 0;
    for (size_t i = 0; i < 100; ++i) {
        avg += integers[i];
        in << integers[i] << "   \t\t   ,  \t  ";
    }
    in << "0\n";
    avg = avg / static_cast<DoubleType>(integers.size()+1);
    expected << avg << "\n";

    std::stringstream out("");
    StreamEvaluator::evaluateStream(in, out, 2);

    CHECK_EQUAL(expected.str(), out.str());
}

int
main(void)
{
    return UnitTest::RunAllTests();
}
