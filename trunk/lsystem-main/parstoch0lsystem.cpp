#include "precompiled.h"

#include "parstoch0lsystem.h"
#include "randomindex.h"

using namespace AP_LSystem;

ParStoch0LSystem::ParStoch0LSystem( AbstractFile * file ): Abstract0LSystem( file )
{
	RandomIndex::init();
}

ParStoch0LSystem::ParStoch0LSystem( const ParStoch0LSystem & c): Abstract0LSystem( c )
{
}

ParStoch0LSystem & ParStoch0LSystem::operator=( const ParStoch0LSystem & c)
{
    this->Abstract0LSystem::operator=( c );
    return *this;
}

multimap<char, Rule>::iterator * ParStoch0LSystem::selectRule(multimap<char, Rule>::iterator & begin,
                                                                     multimap<char, Rule>::iterator & end,
																	 LongString * word,
																	 unsigned int & pos,
                                                                     double * parameters)
{
	int parCnt = 0;
    // ziskej paramtery z pozice za pismenem
    if( !word->getParameters<double>( pos, parameters, parCnt ))
    {
        return NULL;
    }

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


