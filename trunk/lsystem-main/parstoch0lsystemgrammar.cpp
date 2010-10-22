#include "precompiled.h"

#include "parstoch0lsystemgrammar.h"
#include "randomindex.h"

using namespace AP_LSystem;

ParStoch0LSystemGrammar::ParStoch0LSystemGrammar( AbstractFile * file )
{
    this->loadFromFile( file );
}

multimap<char, Rule>::iterator * ParStoch0LSystemGrammar::selectRule(multimap<char, Rule>::iterator & begin,
                                                                     multimap<char, Rule>::iterator & end,
                                                                     double * parameters)
{
    multimap<char, Rule>::iterator * it;
    // rules tha pass the condition and that are processed by random generator
    vector< multimap<char, Rule>::iterator * > passedRules;

    // randomly select rule and take probability into account
    if(begin->second.probabilityFactor)
    {
        RandomIndex ri;
        for( *it = begin; *it != end; (*it)++ )
        {
            if( ((*it)->second.probabilityFactor) && ((*it)->second.evaluateCondition( parameters ) ) )
            {
                ri.addProbability( (*it)->second.probabilityFactor->Eval( parameters ) );
                passedRules.push_back( it );
            }
        }
        // select randomly
        return passedRules[ri.getRandomIndex()];
    }
    else
    {
        // no probability factor - select rule that fulfil the condition
        for( *it = begin; *it != end; (*it)++ )
        {
            //Log::write("variables" + ruleIt->second.variables);
            if( (*it)->second.evaluateCondition( parameters ) )
            {
                return it;
            }
        }
    }

    return NULL;
}

bool ParStoch0LSystemGrammar::nextIteration( )
{
    int j=0;
    char * pUByte = NULL;
    LongString * newWord = new LongString( );
    multimap<char, Rule>::iterator ruleIt;
    pair<multimap<char, Rule>::iterator, multimap<char, Rule>::iterator > result;

    double parameters[100];
    double * pParams = parameters; // parameters pointer
    int parCnt = 0; // parameters counter

/*    for(ruleIt = _rules.begin(); ruleIt != _rules.end(); ruleIt++ )
    {
        stringstream s;

        s << "RULE: " << ruleIt->first << ' ';
        if(ruleIt->second.condition)
            s << "Condition ";
        else
            s << "No condition ";

        if(ruleIt->second.probabilityFactor)
            s << "prob ";
        else
            s << "No prob ";
        s << ' ' << (*(ruleIt->second.staticStrings.begin( )))->toString( );
        Log::write( s.str() );
    }*/

    for(unsigned int i = 0; i < _word->length(); i++ )
    {
        // mozna dodat kontrolu estli jde o pismeno
        result = _rules.equal_range( (*_word)[i]);

        // not found
        if( result.first == result.second )
        {
            j = i;
            pUByte = _word->getSymbol(i);
            newWord->appendData(pUByte,i-j+1);
        }
        // found
        else
        {
            parCnt = 0;
            // ziskej paramtery z pozice za pismenem
            if( !_word->getParamaters( i, pParams, parCnt ))
            {
                return false;
            }

            ruleIt = *selectRule( result.first, result.second, pParams );
            generateSuccessor( newWord, ruleIt, pParams );
        }

        Log::write(newWord->toString());
    }

    if(_word)
        delete _word;

    _word = newWord;

    return true;
}
