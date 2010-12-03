#ifndef RULE_H_
#define RULE_H_

#include <vector>
#include "fparser/fparser.hh"
#include "staticstring.h"

using std::string;

namespace AP_LSystem {
/**
  * Structure for storing rule data. Common for all L-system.
  */
struct Rule
{
    char strictPredecessor;                         ///< strict predecessor of rule
    FunctionParser* condition;                      ///< rule condition - for parametric L-system
    std::string variables;                          ///< list of variables in string
    FunctionParser * probabilityFactor;             ///< probability factor - for stochastic L-system
    std::vector<StaticString*> staticStrings;       ///< static parts of successor
    std::vector<FunctionParser*> dynamicStrings;    ///< dynamic parts of successor
    string leftContext;                             ///< right context
    string rightContext;                            ///< left context

    /**
      * Non-parametric constructor
      */
    Rule():condition(NULL),variables(""),probabilityFactor(NULL), leftContext(""), rightContext(""){}

    /**
      * Copy constructor
      */
    Rule(const Rule &);

    /**
      * Assignment operator
      */
    Rule& operator=( const Rule & );

    /**
      * Destructor
      */
    ~Rule();

    void processProbabilityFactor(string *, string::iterator & );
    void processCondition(string * rule, string::iterator & it);
    void processParameters(string * rule, string::iterator & it);

    bool addStaticString(string * rule, string::iterator & begin);
    void addDynamicString(string * rule, string::iterator & begin);

    bool evaluateCondition( double * p );

private:
    /**
      * This method adds query functions to FunctionParser instance. Adds only if passed string
      * contains this fucntion. Otherwise it optimalize the string.
      */
    void addQueryFunctions( FunctionParser *, string & );
};
}

#endif
