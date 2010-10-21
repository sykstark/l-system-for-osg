#include "precompiled.h"

#include "parstoch0lsystemgrammar.h"
#include "randomindex.h"

using namespace AP_LSystem;

ParStoch0LSystemGrammar::ParStoch0LSystemGrammar( AbstractFile * file )
{
    this->loadFromFile( file );
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

            // randomly select rule and take probability into account
            if(result.first->second.probabilityFactor)
            {
                RandomIndex ri;
                for( ruleIt = result.first; ruleIt != result.second; ruleIt++ )
                {
                    if( ruleIt->second.probabilityFactor == NULL )
                    {
                        // error - probabil. factor is not set
                        return false;
                    }

                    if( ruleIt->second.evaluateCondition( pParams ) )
                    {
                        ri.addProbability( ruleIt->second.probabilityFactor->Eval( pParams ) );
                    }
                }
                ruleIt = result.first;
                unsigned int r = ri.getRandomIndex();
                for(unsigned int k=0; k < r; ruleIt++, k++ );
            }
            else
            {
                // no probability factor - select rule that fulfil the condition
                for( ruleIt = result.first; ruleIt != result.second; ruleIt++ )
                {
                    //Log::write("variables" + ruleIt->second.variables);
                    if( ruleIt->second.evaluateCondition( pParams ) )
                    {
                        break;
                    }
                }
            }

            generateSuccessor( newWord, ruleIt, pParams );
        }

        Log::write(newWord->toString());
    }

    if(_word)
        delete _word;

    _word = newWord;

    return true;
}
