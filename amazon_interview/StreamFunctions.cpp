#include "StreamFunctions.h"

#include <sstream>


////////////////////////////////////////////////////////////////////////////////
// Sum function
void
SumFunction::resetForNewData(void)
{
    // we want to establish that Sum([]) == 0?... yes, it probably make sense.
    mAccum = 0;
}

void
SumFunction::pushInputValues(const std::vector<IntegerType>& streamInput)
{
    for (size_t i = 0, size = streamInput.size(); i < size; ++i) {
        mAccum += streamInput[i];
    }
}

std::string
SumFunction::simEvaluation(void)
{
    // ugly :(
    std::stringstream ss;
    ss << mAccum;
    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////
// Min function

void
MinFunction::resetForNewData(void)
{
    // ugly way to avoid putting a min value that we don't know a priory time
    //... we don't know if std::numeric_limits<IntegerType>::min() will works
    mInitialized = false;
    mMin = 0; // we will return this value as default if we never evaluate this
              // function
}

void
MinFunction::pushInputValues(const std::vector<IntegerType>& streamInput)
{
    if (streamInput.empty()) {
        return;
    }
    if (!mInitialized) {
        mInitialized = true;
        mMin = streamInput.front();
    }
    // find the minimum now
    for (size_t i = 0, size = streamInput.size(); i < size; ++i) {
        if (streamInput[i] < mMin) {
            mMin = streamInput[i];
        }
    }
}

std::string
MinFunction::simEvaluation(void)
{
    // if this wasn't evaluated we should return.... ? NaN?
    if (!mInitialized) {
        return "NaN";
    }
    // ugly :(
    std::stringstream ss;
    ss << mMin;
    return ss.str();
}


////////////////////////////////////////////////////////////////////////////////
// Average function

void
AvgFunction::resetForNewData(void)
{
    mSamples = 0;
    mAccum = 0;
}

void
AvgFunction::pushInputValues(const std::vector<IntegerType>& streamInput)
{
    if (streamInput.empty()) {
        // nothing to do
        return;
    }

    // calculate the new avg

}

std::string
AvgFunction::simEvaluation(void)
{
    // check if we have received some input
    if (mSamples == 0) {
        // we want to establish that Avg([]) == 0?... yes, it probably make sense also.
        return "0.0";
    }

    // now we need to calculate the avg
    // We will use a double to avoid rounding problems (again, here is bad if
    // the size of the integers is really big, we need to use a new "double" type
    // that can support this kind of integers
    DoubleType mAvg = static_cast<DoubleType>(mAccum) /
        static_cast<DoubleType>(mSamples);

    // now print the value into the string, in a ugly way as before :(
    std::stringstream ss;
    ss << mAvg;
    return ss.str();
}


////////////////////////////////////////////////////////////////////////////////
// Percentil90 function
// TODO! read the algorithm and implement this function
class P90Function : public StreamFunction
{
public:
    P90Function() :
        StreamFunction("p90")
    {}

    virtual
    ~P90Function() {};

    // Inherited methods
    //
    virtual void
    resetForNewData(void);

    virtual void
    pushInputValues(const std::vector<IntegerType>& streamInput);

    virtual std::string
    simEvaluation(void);

private:
};
