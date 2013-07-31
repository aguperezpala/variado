#ifndef STREAMEVALUATOR_H_
#define STREAMEVALUATOR_H_


#include <istream>
#include <ostream>

// The purpose of this module is to evaluate a stream using the FunctionEvaluator
// and the Parser already created.
// We will do this in a separate module just to be able to crate some unit tests
// and evaluate it over a generic stream.
//

namespace StreamEvaluator
{

// Define the possible error strings for different cases:
//

// For when we have a invalid format in the line we are parsing...
static const char* INVALID_LINE_FORMAT = "ILF";

// For when we parse a unknown function
static const char* UNKNOWN_FUNCTION_NAME = "UFN";

// For when there is just a simple \n without data or without the integers list
static const char* EMPTY_LINE = "EL";



// @brief Evaluate a input stream and put all the results on a given output
//        stream.
//        Here it will be implemented all the logic of how we should handle
//        the errors and any kind of problem.
// @param iStream   The input stream
// @param oStream   The output stream where the results will be inserted.
// @param maxInt    The number of how much integers we want to parse at once.
//                  This is used to avoid read all the line at once since we don't
//                  know the size at priori
//
void
evaluateStream(std::istream& iStream, std::ostream& oStream, unsigned int maxInt);

}

#endif /* STREAMEVALUATOR_H_ */
