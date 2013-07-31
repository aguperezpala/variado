#include "StreamEvaluator.h"

#include <vector>

#include "Defines.h"
#include "Parser.h"
#include "FunctionBuilder.h"
#include "Debug.h"


// Auxiliary methods
//
namespace {

// Parse a function. Returns true if we have to continue (everything ok) or
// false otherwise
//
bool
parseFunction(std::istream& iStream,
              std::ostream& oStream,
              Parser& parser,
              std::string& funName,
              Parser::ResultCode& code)
{
    code = parser.parseFunction(funName);
    switch (code) {
    case Parser::ResultCode::NoError: return true; //we do nothing
    case Parser::ResultCode::InvalidFormat:
        // print an invalid format and skip the current line
        oStream << StreamEvaluator::INVALID_LINE_FORMAT << '\n';
        code = parser.skipCurrentLine();
        return false;
    case Parser::ResultCode::EndOfLine:
        // we have an empty line...
        oStream << StreamEvaluator::EMPTY_LINE << '\n';
        return false;
    case Parser::ResultCode::EndOfFile:
        // exit
        return false;
    default:
        // ?
        ;
    }

    ASSERT(false && "We reach an impossible state?\n");
    return false;
}


// Function that parse the list of numbers and check for the error.
// If something fails will return false, if everything is fine will return true
//
bool
parseNumbers(std::istream& iStream,
             std::ostream& oStream,
             Parser& parser,
             unsigned int maxInt,
             std::vector<IntegerType>& integers,
             Parser::ResultCode& code)
{
    // clear the data just in case
    integers.clear();

    code = parser.parseNumbers(maxInt, integers);
    switch (code) {
        case Parser::ResultCode::NoError: return true; // we do nothing else here
        case Parser::ResultCode::InvalidFormat:
            // print an invalid format and skip the current line
            oStream << StreamEvaluator::INVALID_LINE_FORMAT << '\n';
            code = parser.skipCurrentLine();
            return false;
        case Parser::ResultCode::EndOfLine:
            // we need to check if we could read at least one value
            if (integers.empty()) {
                // we have an empty line without integers...
                oStream << StreamEvaluator::EMPTY_LINE << '\n';
                return false;
            } else {
                // we read some values.. so we need to continue
                return true;
            }
        case Parser::ResultCode::EndOfFile:
            // exit
            return false;
        default:
            ASSERT(false && "We reach an impossible state?\n");
        }

    return false;
}


}

namespace StreamEvaluator {

void
evaluateStream(std::istream& iStream, std::ostream& oStream, unsigned int maxInt)
{
    Parser parser(iStream);
    FunctionBuilder fBuilder;
    Parser::ResultCode code = Parser::ResultCode::NoError;
    std::string funName;
    std::vector<IntegerType> integers;
    integers.reserve(maxInt);

    while (!iStream.eof() && code != Parser::ResultCode::EndOfFile) {
        // first parse the function name
        if (!parseFunction(iStream, oStream, parser, funName, code)) {
            // something fails...
            continue;
        }

        // get the function
        StreamFunction* function = fBuilder.getFunction(funName);
        if (function == 0) {
            // unknown function name
            oStream << UNKNOWN_FUNCTION_NAME << '\n';
            code = parser.skipCurrentLine();
            continue;
        }


        // now start parsing the numbers and evaluate the function
        function->resetForNewData();
        while (parseNumbers(iStream, oStream, parser, maxInt, integers, code)) {
            // stream the input into the function
            function->pushInputValues(integers);

            // we also need to check if the current code is a new line and
            // we already read all the values.. to stop reading anything else
            if (code == Parser::ResultCode::EndOfLine) {
                ASSERT(!integers.empty());
                break;
            }
        }

        // now check for the last code we got to see if we need to evaluate or
        // not the function
        if (code != Parser::ResultCode::EndOfLine ||
            (integers.empty() && code == Parser::ResultCode::EndOfLine)) {
            // we have nothing to do since we always we finish with a EndOfLine..
            continue;
        }

        // if we are here then we should evaluate the function and put the
        // result into the output stream
        oStream << function->simEvaluation() << '\n';
    }

}

}

