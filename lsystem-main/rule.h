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

    /**
      * Parses string, processes and adds probability factor
      * @param rule string to parse
      * @param it opening probability factor position in rule string
      */
    void processProbabilityFactor(string * rule, string::iterator & it);

    /**
      * Parses string, processes and adds condition
      * @param rule string to parse
      * @param it opening condition position in rule string
      */
    void processCondition(string * rule, string::iterator & it);

    /**
      * Parses string, processes and adds parameter variables of predecessor or context
      * @param rule string to parse
      * @param it opening variables position in rule string
      */
    void processParameters(string * rule, string::iterator & it);

    /**
      * Parses string, processes and adds static part of successor
      * @param rule string to parse
      * @param it opening static string position in rule string
      * @return - false - if successor end behind this parsed part
      *         - true - if successor continues with expression behind this static part
      */
    bool addStaticString(string * rule, string::iterator & it);

    /**
      * Parses string, processes and adds dynamic part(expressions) of successor
      * @param rule string to parse
      * @param it opening variables position in rule string
      */
    void addDynamicString(string * rule, string::iterator & it);

    /**
      * Evaluates rule's condition.
      * @param p array of variables values
      * @return result of evaluation
      */
    bool evaluateCondition( double * p );

private:
    /**
      * This method adds query functions to FunctionParser instance. Adds only if passed string
      * contains this fucntion. Otherwise it optimalize the string.
      * @param expression pointer to expression function parser instance. Query functions will
      *         be added here
      * @param expressionString original expression string
      */
    void addQueryFunctions( FunctionParser * expression, string & expressionString);
};
}

#endif
