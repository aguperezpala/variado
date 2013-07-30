#ifndef STREAMFUNCTION_H_
#define STREAMFUNCTION_H_

#include <string>
#include <vector>

#include "Defines.h"

// Here we will define the interface that the different functions should have.
// We will create a "StreamFunction" that means a function that could be
// calculated with multiple calls (the function can take the input values from
// multiple consecutive calls, this means that the function doesn't need all
// the input information at once to be evaluated.... Something that probably
// is not mathematically right but we need to do this because we don't know
// the number of the input values at priori).
//

class StreamFunction
{
public:
    // @brief Get the name (optional) of this function
    //
    StreamFunction(const std::string& name = "") :
        mName(name)
    {}

    virtual
    ~StreamFunction() {};

    // @brief Set / get function name
    //
    inline const std::string&
    name(void) const;
    inline void
    setName(const std::string& name);

    // @brief Method that should be called before we want to start to evaluate
    //        the function. This will initialize the function to calculate the
    //        new output from a new set of values.
    //
    virtual void
    resetForNewData(void) = 0;

    // @brief This method will be used to add the input values that this function
    //        should take. For these cases all the values will IntegerType.
    // @param streamInput   The new set of input values that this function takes.
    //
    virtual void
    pushInputValues(const std::vector<IntegerType>& streamInput) = 0;

    // @brief This method will be called once we finish the "streaming" of input
    //        values. And we will return the calculated value for all the
    //        streamed input (this will simulate an evaluation of the function).
    // @return The result of the evaluation of this function.
    // @note  We sadly need to return a (inefficient and ugly) string because
    //        one more time we don't know the output type value we should return
    //        for each function at priory.
    //
    virtual std::string
    simEvaluation(void) = 0;

private:
    std::string mName;

};



///////////////////////////////////////////////////////////////////////////////
// Inline stuff
//

inline const std::string&
StreamFunction::name(void) const
{
    return mName;
}
inline void
StreamFunction::setName(const std::string& name)
{
    mName = name;
}


#endif /* STREAMFUNCTION_H_ */
