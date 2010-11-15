#include "precompiled.h"

#include "par2lsystem.h"

using namespace AP_LSystem;

Par2LSystem::Par2LSystem( AbstractFile * file )
{
	this->loadFromFile( file );
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

    // insert new rule into map with rules
    this->_rules.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
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
    this->_homomorphisms.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
}

multimap<char, Rule>::iterator * Par2LSystem::selectRule(multimap<char, Rule>::iterator & begin,
                                                                multimap<char, Rule>::iterator & end,
																LongString * word,
																unsigned int & pos,
                                                                double * parameters)
{
    multimap<char, Rule>::iterator * it = new multimap<char, Rule>::iterator;
    
    for( *it = begin; *it != end; (*it)++ )
    {
		int leftContext = pos-1, rightContext = pos+1, paramCount=0;
		
		// exist left context ?
		if((*it)->second.leftContext.length())
		{	
			// TODO consider
			// match left context

			_word->peekSymbol(leftContext, false);
			leftContext = _word->matchLeft( (*it)->second.leftContext[0], leftContext, &ignore, NULL );
			// left context doesn't match
			if( leftContext < 0 )
				continue;
		}
		
		// exist right context ?
		if((*it)->second.rightContext.length())
		{
			// match right context
			_word->peekSymbol(rightContext, true);
			rightContext = _word->matchRight( (*it)->second.rightContext[0], rightContext, &ignore, NULL );
			// right context doesn't match
			if( rightContext < 0 )
				continue;
		}

		// get parameters of left context
		unsigned con = leftContext;
		if( !word->getParameters<double>( con, parameters, paramCount ))
		{
			return NULL;
		}
		// append parameters of nonterminal
		if( !word->getParameters<double>( pos, parameters, paramCount ))
		{
			return NULL;
		}
		// append parameters of right context
		con = rightContext;
		if( !word->getParameters<double>( con, parameters, paramCount ))
		{
			return NULL;
		}	
		
		//evaluate condition
        if( (*it)->second.evaluateCondition( parameters ) )
        {
            return it;
        }
    }

    return NULL;
}
