#include "FunctionBuilder.h"

#include <cctype>
#include <math>

#include "Debug.h"
// include all the functions here
#include "StreamFunctions.h"


// helper methods
namespace {

// Check if two strings are equal using toLower
bool
areStringsEquals(const std::string& s1, const std::string& s2)
{
    if (s1.size() != s2.size()) {
        return false;
    }
    size_t min = std::min(s1.size(), s2.size());
    for (size_t i = 0; i < min; ++i) {
        if (std::tolower(s1[i]) != std::tolower(s2[i])) {
            // don't match!
            return false;
        }
    }

    // everything fine!
    return true;
}
}


////////////////////////////////////////////////////////////////////////////////
FunctionBuilder::FunctionBuilder()
{
    // create the functions here
    mFunctions.reserve(4);
    mFunctions.push_back(new SumFunction);
    mFunctions.push_back(new MinFunction);
    mFunctions.push_back(new AvgFunction);
    mFunctions.push_back(new P90Function);
}

////////////////////////////////////////////////////////////////////////////////
FunctionBuilder::~FunctionBuilder()
{
    // a better way can be using a shared_ptr here, but we don't want to force
    // to use the new standard nor boost (nor create a own wrapper just for this..)
    for (size_t i = 0, size = mFunctions.size(); i < size; ++i) {
        ASSERT(mFunctions[i]);
        delete mFunctions[i];
    }
}

////////////////////////////////////////////////////////////////////////////////
bool
FunctionBuilder::checkFunctionExists(const std::string& funName) const
{
    // get the function, code dup :(
    for (size_t i = 0, size = mFunctions.size(); i < size; ++i) {
        if (areStringsEquals(funName, mFunctions[i]->name())) {
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
StreamFunction*
FunctionBuilder::getFunction(const std::string& funName)
{
    for (size_t i = 0, size = mFunctions.size(); i < size; ++i) {
        if (areStringsEquals(funName, mFunctions[i]->name())) {
            return mFunctions[i];
        }
    }

    // no function found
    return 0;
}
