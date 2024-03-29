// TODO: remove this
#include <iostream>

#include "Parser.h"

#include "Debug.h"

////////////////////////////////////////////////////////////////////////////////
bool
Parser::readUntil(const std::string& chars, char& charFound)
{
    while (mStream >> charFound) {
        if (chars.find(charFound) != std::string::npos) {
            // we found the character we want
            return true;
        }
    }
    // we couldn't find anything and get an error before.
    return false;
}

////////////////////////////////////////////////////////////////////////////////
int
Parser::skipWhiteSpacesExcept(char exception)
{
    while (mStream.good()) {
        const int current = mStream.peek();
        if (current == exception || !std::isspace(current)) {
            // if is the exception we should remove it
            if (current == exception) {
                mStream.ignore();
            }

            return current;
        }
        // we should remove the last space character
        mStream.ignore();
    }

    return -1;
}
////////////////////////////////////////////////////////////////////////////////
bool
Parser::readNumber(IntegerType& result, char& lastChar)
{
    // skip spaces
    int nextCharacter = skipWhiteSpacesExcept('\n');
    if (nextCharacter < 0) {
        // some error occurr
        debugERROR("The line is ill-formed? %c\n", static_cast<char>(nextCharacter));
        return false;
    }
    lastChar = nextCharacter;
    if (lastChar == '\n') {
        // nothing to read
        return false;
    }

    // now we will try to read the number... if we can't is because something
    // is ill formed
    if (!(mStream >> result)) {
        // we couldn't read the number
        return false;
    }
    // read until the next not white space character or the NUM_SEPARATOR_CHAR
    nextCharacter = skipWhiteSpacesExcept('\n');
    if (nextCharacter < 0) {
        // some error occurr
        debugERROR("The line is ill-formed? %c\n", static_cast<char>(nextCharacter));
        return false;
    }

    // check if we read the NUM_SEPARATOR_CHAR or a \n
    lastChar = nextCharacter;

    // check if we have to remove the last character from the stream
    if (lastChar == NUM_SEPARATOR_CHAR) {
        mStream.ignore();
    }


    return (lastChar == NUM_SEPARATOR_CHAR || lastChar == '\n') ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Parser::Parser(std::istream& stream) :
    mStream(stream)
{
    mStream >> std::noskipws;
}

////////////////////////////////////////////////////////////////////////////////
Parser::~Parser()
{
    // do nothing
}

////////////////////////////////////////////////////////////////////////////////
Parser::ResultCode
Parser::parseFunction(std::string& funName)
{
    funName.clear();

    // clear the white spaces except for \n
    const int firstChar = skipWhiteSpacesExcept('\n');
    if (firstChar < 0) {
        // some error occur
        debugERROR("Some error occurr when reading the first character?\n");
        return (mStream.eof()) ? ResultCode::EndOfFile : ResultCode::InvalidFormat;
    }

    if (firstChar == '\n') {
        // we found the new line, there was not a function here...
        return ResultCode::EndOfLine;
    }

    // we already read the first character, we can read the next characters that
    // conforms the function name, but we should throw away that character first
    mStream.ignore();
    funName.push_back(firstChar);
    for (size_t i = 1; i < MAX_FUN_NAME; ++i) {
        const int currentChar = mStream.get();
        // check that is a valid character (not a space)
        if (std::isspace(currentChar) || currentChar < 0) {
            debugERROR("Incalid function name: %s, problematic character: %c\n",
                    funName.c_str(), static_cast<char>(currentChar));
            return ResultCode::InvalidFormat;
        }
        funName.push_back(currentChar);
    }

    // now we need to get the FUN_SEPARATOR_CHAR to be sure that we are getting
    // the function name correctly
    const int sepCharacter = skipWhiteSpacesExcept(FUN_SEPARATOR_CHAR);

    if (sepCharacter != FUN_SEPARATOR_CHAR) {
        // something was wrong?
        debugERROR("Error trying to find the FUN_SEPARATOR_CHAR, we found %c\n",
                static_cast<char>(sepCharacter));
        return (sepCharacter == '\n') ? ResultCode::EndOfLine : ResultCode::InvalidFormat;
    }

    // we now are ready to return the correct function name and we had left
    // the stream pointing just after the FUN_SEPARATOR_CHAR
    return ResultCode::NoError;
}

////////////////////////////////////////////////////////////////////////////////
Parser::ResultCode
Parser::parseNumbers(unsigned int N, std::vector<IntegerType>& numbers)
{
    if (N == 0) {
        debugWARNING("We are asking to parse 0 numbers...\n");
        return ResultCode::NoError;
    }

    // clear the input data
    numbers.clear();

    // skip the first spaces... and check if we had a list...
    const int firstChar = skipWhiteSpacesExcept('\n');
    if (firstChar < 0) {
        // some error occur
        debugERROR("Some error occur when reading the first character?\n");
        return (mStream.eof()) ? ResultCode::EndOfFile : ResultCode::InvalidFormat;
    }

    if (firstChar == '\n') {
        // we found the new line, we haven't a list of numbers
        return ResultCode::EndOfLine;
    }

    // now we can read the next N numbers
    bool everythingOK = true;
    char lastChar = 0;
    IntegerType number;
    numbers.reserve(N); // just in case to avoid multiple reallocations

    for (unsigned int i = 0; i < N && everythingOK; ++i) {
        everythingOK = readNumber(number, lastChar) && lastChar != '\n';
        if (everythingOK || (!everythingOK && (lastChar == '\n'))) {
            numbers.push_back(number);
        }
    }

    // check if we go out because lastCharacter was a \n
    if (!everythingOK) {
        if (lastChar == '\n') {
            // we could read a lot of numbers :)
            return ResultCode::EndOfLine;
        } else {
            // something was wrong then...
            return ResultCode::InvalidFormat;
        }
    }

    // we could read all the numbers we ask for or less than
    ASSERT(numbers.size() <= N);
    return ResultCode::NoError;
}



Parser::ResultCode
Parser::skipCurrentLine(void)
{
    char dummy;
    if (!readUntil("\n", dummy)) {
        // some problem occurr ?
        return (mStream.eof()) ? ResultCode::EndOfFile : ResultCode::InvalidFormat;
    }

    // everything fine
    return ResultCode::NoError;
}

