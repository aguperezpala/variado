#ifndef FUNCTIONBUILDER_H_
#define FUNCTIONBUILDER_H_

#include <string>

#include "StreamFunction.h"

// This class will hide the number of functions we can have and handle all the
// possible functions we can work with.
//

class FunctionBuilder {
public:
    // constructor / destructor
    FunctionBuilder();
    ~FunctionBuilder();

    // @brief Check for the existence of a function from a given name.
    //        We will transform the name to lower case always.
    // @param funName   The function name we want to check.
    // @return true if the function exists, false otherwise
    //
    bool
    checkFunctionExists(const std::string& funName) const;

    // @brief Return the associated StreamFunction instance from a given name.
    //        Note that you should reset the function (resetForNewData()) before
    //        use it, since we always work with the same instance of the function.
    // @param funNanme  The function name of the StreamFunction we want to get.
    // @return the instance of the StreamFunction or 0 if the function doesnt'
    //         exists
    // @note   Do not free the memory of this object. This class is the owner
    //
    StreamFunction*
    getFunction(const std::string& funName);

private:
    std::vector<StreamFunction*> mFunctions;
};

#endif /* FUNCTIONBUILDER_H_ */
