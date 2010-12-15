#include "precompiled.h"

#include "par2lsystem.h"
#include "randomindex.h"

using namespace AP_LSystem;

Par2LSystem::Par2LSystem( AbstractFile * file ): AbstractkLSystem( file )
{
    this->loadFromFile( file );
	RandomIndex::init();
}

void Par2LSystem::processPredecessor(Rule & r, string * rule, string::iterator & it)
{
    // process a rule
    // example: B(k,l,m)<A(x,y)>C(z):x<y+k->B(l+1,y-2,z+m)

    if( *it != '<' )
    {
        // left context
        // example: B
        r.leftContext.append( 1, *it++ );

        // parameters of left context
        // example: (k,l,m)
        r.processParameters(rule, it);

        // left context sign
        // example: <
        if(*it++ != '<')
        {
            throw ParsingException("Symbol \'<\' was expected!");
        }
    }
    else
    {
        it++;
    }

    // nonterminal
    // example: A
    r.strictPredecessor = *it++;

    // parameters of nonterminal
    // example: (x,y)
    r.processParameters(rule, it);

    // right context sign
    // example: >
    if(*it++ != '>')
    {
        throw ParsingException("Symbol \'<\' was expected!");
    }

    if( (*it != ':') && ( (*it != '-') || (*(it+1) != '>') ) )
    {
        // right context
        // example: B
        r.rightContext.append( 1, *it++ );

        // parameters of right context
        // example: (k,l,m)
        r.processParameters(rule, it);
    }

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

void Par2LSystem::processRuleSuccessor(Rule & r, string * rule, string::iterator & it)
{
    // look for opening bracket - end of static string
    //		each rule has to start with static string

    // slo by zrychlit testem na parametric - pridat rovnou cely retezec

    while( r.addStaticString(rule, it) )
    {
        r.addDynamicString(rule, it);
    }

	r.processProbabilityFactor(rule, it);

    // insert new rule into map with rules
    this->m_Rules.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
}

void Par2LSystem::processHomomorphismSuccessor(Rule & r, string * hom, string::iterator & it)
{
    // homomorphism successor
    // example: +F(x)
    r.addStaticString(hom, it);

    // probability factor
    // example: :max(1,y)
    r.processProbabilityFactor(hom, it);

    // insert new rule into map with rules
    this->m_Homomorphisms.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
}

multimap<char, Rule>::iterator * Par2LSystem::selectRule(multimap<char, Rule>::iterator & begin,
                                                                multimap<char, Rule>::iterator & end,
																LongString * word,
																unsigned int & pos,
                                                                double * parameters)
{
    multimap<char, Rule>::iterator * it = new multimap<char, Rule>::iterator;
	// rules tha pass the condition and that are processed by random generator
    vector< multimap<char, Rule>::iterator > passedRules;
	RandomIndex ri;
	unsigned predecessor, context;
    
    for( *it = begin; *it != end; (*it)++ )
    {
		int leftContext = pos-1, rightContext = pos+1, paramCount=0;
		
		// exist left context ?
		if((*it)->second.leftContext.length())
		{	
			// TODO consider

			// match left context
			m_Word->peekSymbol(leftContext, false);
			leftContext = m_Word->matchLeft( (*it)->second.leftContext[0], leftContext, &m_Ignore, NULL );
			// left context doesn't match
			if( leftContext < 0 )
				continue;
			else
			{
				// convert int to unsigned int
				context = leftContext;
				// get parameters of left context
				if( !word->getParameters<double>( context, parameters, paramCount ))
					return NULL;
			}
		}

		// append parameters of nonterminal
		predecessor = pos;
		if( !word->getParameters<double>( predecessor, parameters, paramCount ))
		{
			return NULL;
		}
		
		// exist right context ?
		if((*it)->second.rightContext.length())
		{
			// match right context
			m_Word->peekSymbol(rightContext, true);
			rightContext = m_Word->matchRight( (*it)->second.rightContext[0], rightContext, &m_Ignore, NULL );
			// right context doesn't match
			if( rightContext < 0 )
				continue;
			else
			{
				// convert int to unsigned int
				context = rightContext;
				// append parameters of right context
				if( !word->getParameters<double>( context, parameters, paramCount ))
					return NULL;
			}
		}		
		
		//evaluate condition
        if( (*it)->second.evaluateCondition( parameters ) )
        {
			if((*it)->second.probabilityFactor)
			{
				double eval = (*it)->second.probabilityFactor->Eval( parameters );
				ri.addProbability( eval );
				//vrecko::logger.debugLog( "k = %f, eval = %f", parameters[0], eval );
				passedRules.push_back( *it );
			}
			else
			{
				// set pos behind this symbol
				pos = predecessor;
				return it;
			}
        }
    }
	// select randomly
	if(passedRules.empty())
	{
		return NULL;
	}
	else
	{
		// set pos behind this symbol
		pos = predecessor;
		*it = passedRules[ri.getRandomIndex()];
		return it;
	}
}
