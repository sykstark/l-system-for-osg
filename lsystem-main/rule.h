#ifndef RULE_H_
#define RULE_H_

#include <vector>
#include "fparser/fparser.hh"
#include "staticstring.h"

using std::string;

namespace AP_LSystem {
struct Rule
{
    char strictPredecessor;
	FunctionParser* condition;
	std::string variables;
    FunctionParser * probabilityFactor;
    std::vector<StaticString*> staticStrings;
	std::vector<FunctionParser*> dynamicStrings;
	string leftContext;
	string rightContext;

    Rule():condition(NULL),variables(""),probabilityFactor(NULL), leftContext(""), rightContext(""){}

    Rule(const Rule &);
    Rule& operator=( const Rule & );
    ~Rule();

    void processProbabilityFactor(string *, string::iterator & );
    void processCondition(string * rule, string::iterator & it);
    void processParameters(string * rule, string::iterator & it);

    bool addStaticString(string * rule, string::iterator & begin);
    void addDynamicString(string * rule, string::iterator & begin);

    bool evaluateCondition( double * p );

private:
    void addQueryFunctions( FunctionParser *, string & );
};
}

#endif
