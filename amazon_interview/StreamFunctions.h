#ifndef STREAMFUNCTIONS_H_
#define STREAMFUNCTIONS_H_


#include "Defines.h"
#include "StreamFunction.h"



////////////////////////////////////////////////////////////////////////////////
// Here we will define all the possible functions we will use. Note that this
// is not a good way to do it since we are coupling different functions but for
// this small project doesn't matters (to avoid having a lot of files).
//


////////////////////////////////////////////////////////////////////////////////
// Sum function
class SumFunction : public StreamFunction
{
public:
    SumFunction() :
        StreamFunction("sum")
    ,   mAccum(0)
    {}

    virtual
    ~SumFunction() {};

    // Inherited methods
    //
    virtual void
    resetForNewData(void);

    virtual void
    pushInputValues(const std::vector<IntegerType>& streamInput);

    virtual std::string
    simEvaluation(void);

private:
    // to avoid a possible overflow we can use long long ints... but this way
    // we probably are not being complaint with the IntegerType..

    IntegerType mAccum;
};

////////////////////////////////////////////////////////////////////////////////
// Min function
class MinFunction : public StreamFunction
{
public:
    MinFunction() :
        StreamFunction("min")
    ,   mMin(0)
    ,   mInitialized(false)
    {}

    virtual
    ~MinFunction() {};

    // Inherited methods
    //
    virtual void
    resetForNewData(void);

    virtual void
    pushInputValues(const std::vector<IntegerType>& streamInput);

    virtual std::string
    simEvaluation(void);

private:
    IntegerType mMin;
    bool mInitialized;
};

////////////////////////////////////////////////////////////////////////////////
// Average function
class AvgFunction : public StreamFunction
{
public:
    AvgFunction() :
        StreamFunction("avg")
    ,   mAccum(0)
    ,   mSamples(0)
    {}

    virtual
    ~AvgFunction() {};

    // Inherited methods
    //
    virtual void
    resetForNewData(void);

    virtual void
    pushInputValues(const std::vector<IntegerType>& streamInput);

    virtual std::string
    simEvaluation(void);

private:
    IntegerType mAccum;
    IntegerType mSamples;
};

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

#endif /* STREAMFUNCTIONS_H_ */
