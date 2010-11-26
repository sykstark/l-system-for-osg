#include "precompiled.h"

#include "abstract0lsystem.h"

using namespace AP_LSystem;

Abstract0LSystem::Abstract0LSystem( AbstractFile * file ): LSystem(file)
{
    this->loadFromFile( file );
}

void Abstract0LSystem::processPredecessor(Rule & r, string * rule, string::iterator & it)
{
    // process a rule
    // example: A(x,y):x<y->B(x+1,y-2):max(1,x)

    // predecessor
    // example: A
    r.strictPredecessor = *it++;

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
}

void Abstract0LSystem::processRuleSuccessor(Rule & r, string * rule, string::iterator & it)
{
    // look for opening bracket - end of static string
    //		each rule has to start with static string
    while( r.addStaticString(rule, it) )
    {
        r.addDynamicString(rule, it);
    }

    r.processProbabilityFactor(rule, it);

    // insert new rule into map with rules
    this->_rules.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
}

void Abstract0LSystem::processHomomorphismSuccessor(Rule & r, string * hom, string::iterator & it)
{
    // homomorphism successor
    // example: +F(x)
    r.addStaticString( hom, it );

    // probability factor
    // example: :max(1,y)
    r.processProbabilityFactor(hom, it);

    // insert new rule into map with rules
    this->_homomorphisms.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
}

