#ifndef PARSER_H_
#define PARSER_H_

#include <istream>
#include <vector>
#include <string>

// @brief This class will be specialized to parse each line of the input stream.
//        We will not use generic "methods" for parsing since it is a little
//        project and we don't need to do a lot of things here (and we are using
//        STL methods to parse). So this is a "Specialized Wrapper Parser" for our
//        specific porpouse (parse method names and a list of numbers)
//
class Parser {
public:

    // Define the list of errors that the parser can return
    //
    enum ResultCode {
        NoError = 0,
        InvalidFormat,
        EndOfLine,
        EndOfFile,
    };

    // Some constants used for this specific parser
    //
    static const size_t MAX_FUN_NAME = 3;
    static const char FUN_SEPARATOR_CHAR = ':';
    static const char NUM_SEPARATOR_CHAR = ',';

public:

    // @brief Constructor that takes the stream to be used for parsing
    // @param stream    The istream to be used
    //
    Parser(std::istream& stream);
    ~Parser();

    // @brief   Method used to parse a function name starting from the current
    //          position of the stream
    // @param   funName   The name of the function readed, the name should be
    //                  less than MAX_FUN_NAME
    // @return  the associated ResultCode
    //
    ResultCode
    parseFunction(std::string& funName);

    // @brief   Method used to parse a list of numbers starting in the current
    //          position of the stream. We will parse N numbers from the stream
    //          or until we get a '\n'
    // @param   N         Indicates how many numbers we want to read.
    // @param   numbers   The vector of numbers readed.
    // @returns NoError         if succes (and we didn't read a \n)
    //          EndOfLine       if we could read the < N numbers and get the \n
    //          EndOfFile       if we got with and EOF.
    //          InvalidFormat   on error.
    // @note We are assuming that if you call this method you is because
    //       the stream is already pointing to the correct place (i.e you already
    //       read the function name or you read numbers in the last call).
    //
    ResultCode
    parseNumbers(unsigned int N, std::vector<int>& numbers);

    // @brief With this function we can read until the next \n (including).
    //        This way we can avoid having problems when parsing new lines.
    // @return NoError on success | the associated error otherwise
    //
    ResultCode
    skipCurrentLine(void);

    // @brief Helper methods to get / set the current position of the stream
    //
    inline size_t
    streamPosition(void) const;
    inline void
    setStreamPosition(size_t newPos);

    // @brief Returns the current stream used
    //
    inline const std::istream&
    istream(void) const;

private:

    // @brief Read bytes until we get with one inside of the list.
    // @param chars     The list of characters we need to find
    // @param charFound The character found (if we found it), check the return value
    // @return true on succes | false on fail (EOF for example).
    //
    bool
    readUntil(const std::string& chars, char& charFound);

    // @brief Method that skips all the spaces from the stream except
    //        of a particular character.
    // @param exception     The character we don't want to skip
    // @return the last character we found that could be a the exception
    //         character or another character that is not a space
    //         or -1 on error
    //
    int
    skipWhiteSpacesExcept(char exception);

    // @brief Helper method used to parse a number and verify the next character
    // @param result    The readed number.
    // @param lastChar  The last character readed (except for spaces).
    // @returns true on succes | false on error (check stream status)
    //
    bool
    readNumber(int& result, char& lastChar);

private:
    std::istream& mStream;
};




////////////////////////////////////////////////////////////////////////////////
// Inline stuff
inline size_t
Parser::streamPosition(void) const
{
    return mStream.tellg();
}
inline void
Parser::setStreamPosition(size_t newPos)
{
    mStream.seekg(newPos, mStream.beg);
}
inline const std::istream&
Parser::istream(void) const
{
    return mStream;
}


#endif /* PARSER_H_ */
