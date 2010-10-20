#include "precompiled.h"

#include "abstract0lsystemgrammar.h"

using namespace AP_LSystem;

Abstract0LSystemGrammar::Abstract0LSystemGrammar()
{
}

void Abstract0LSystemGrammar::addRule(std::string * rule)
{
    Rule r;
    std::string::iterator it = rule->begin();

    // process a rule
    // example: A(x,y):x<y->B(x+1,y-2):max(1,x)

    // non terminal
    // example: A
    char nonTerminal = *it++;

    // parametric rule
    // example: (x,y)
    r.processParameters(rule, it);

    // condition of rule
    // example: :x<y
    r.processCondition(rule, it);

    // transcryption sign
    // example: ->
    if( (*it++ != '-') || (*it++ != '>') )
    {
        throw ParsingException("Symbol \'->\' was expected!");
    }

    // look for opening bracket - end of static string
    //		each rule has to start with static string

    // slo by zrychlit testem na parametric - pridat rovnou cely retezec

    while( r.addStaticString(rule, it) )
    {
        r.addDynamicString(rule, it);
    }

    r.processProbabilityFactor(rule, it);

//    cout << "static:" << str << endl;

    // insert new rule into map with rules
    this->_rules.insert(make_pair< char, Rule >(nonTerminal, r ));
}

void Abstract0LSystemGrammar::addHomomorphism(std::string * hom)
{
    Rule r;
    std::string::iterator end, it = hom->begin();
    StaticString * pSS;

    // process homomorphism
    // example: A(x,y):x<y->+F(x):max(1,y)

    // non terminal
    // example: A
    char nonTerminal = *it++;

    // parametric rule
    // example: (x,y)
    r.processParameters(hom, it);

    // condition of rule
    // example: :x<y
    r.processCondition(hom, it);

    // transcryption sign
    // example: ->
    if( (*it++ != '-') || (*it++ != '>') )
    {
        throw ParsingException("Symbol \'->\' was expected!");
    }

    // homomorphism successor
    // example: +F(x)
    string str;

    end = hom->begin() + hom->find( ':', it-hom->begin() );
    pSS = new StaticString(string(it, end));
    r.staticStrings.push_back( pSS );

    // probability factor
    // example: :max(1,y)
    r.processProbabilityFactor(hom, it);

    // insert new rule into map with rules
    this->_homomorphisms.insert(make_pair< char, Rule >(nonTerminal, r ));

}

