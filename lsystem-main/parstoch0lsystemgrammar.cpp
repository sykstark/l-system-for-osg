#include "precompiled.h"

#include "parstoch0lsystemgrammar.h"
#include "randomindex.h"

using namespace AP_LSystem;

ParStoch0LSystemGrammar::ParStoch0LSystemGrammar( AbstractFile * file )
{
    this->loadFromFile( file );
	RandomIndex::init();
}

multimap<char, Rule>::iterator * ParStoch0LSystemGrammar::selectRule(multimap<char, Rule>::iterator & begin,
                                                                     multimap<char, Rule>::iterator & end,
                                                                     double * parameters)
{
    multimap<char, Rule>::iterator * it = new multimap<char, Rule>::iterator;
    // rules tha pass the condition and that are processed by random generator
    vector< multimap<char, Rule>::iterator > passedRules;

    // randomly select rule and take probability into account
    if(begin->second.probabilityFactor)
    {
        RandomIndex ri;
        for( *it = begin; *it != end; (*it)++ )
        {
            if( ((*it)->second.probabilityFactor) && ((*it)->second.evaluateCondition( parameters ) ) )
            {
                ri.addProbability( (*it)->second.probabilityFactor->Eval( parameters ) );
                passedRules.push_back( *it );
            }
        }
        // select randomly
		*it = passedRules[ri.getRandomIndex()];
        return it;
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
    multimap<char, Rule>::iterator * pRuleIt;
    pair<multimap<char, Rule>::iterator, multimap<char, Rule>::iterator > result;

    double parameters[100];
    double * pParams = parameters; // parameters pointer
    int parCnt = 0; // parameters counter

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

            pRuleIt = selectRule( result.first, result.second, pParams );

			if(pRuleIt)
			{
				generateSuccessor( newWord, *pRuleIt, pParams );
				delete pRuleIt;
			}
        }

        Log::write(newWord->toString());
    }

    if(_word)
        delete _word;

    _word = newWord;

    return true;
}
