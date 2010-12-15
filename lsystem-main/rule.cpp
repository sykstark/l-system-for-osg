#include "precompiled.h"

#include "rule.h"

#include "boost/lexical_cast.hpp"
#include "lsystemexception.h"
#include "longstring.h"
#include "log.h"
#include "randomizer.h"
#ifdef _MSC_VER
#include "query.h"
#endif

using boost::lexical_cast;
using boost::bad_lexical_cast;
using namespace AP_LSystem;

Rule::Rule( const Rule & c ) : strictPredecessor(c.strictPredecessor),
                               variables( c.variables ),
                               leftContext( c.leftContext ),
                               rightContext( c.rightContext )

{
    for(std::vector< StaticString *>::const_iterator it = c.staticStrings.begin(); it != c.staticStrings.end(); it++)
        staticStrings.push_back( new StaticString(**it) );

    for(std::vector< FunctionParser *>::const_iterator it = c.dynamicStrings.begin(); it != c.dynamicStrings.end(); it++)
        dynamicStrings.push_back( new FunctionParser(**it) );

    probabilityFactor = (c.probabilityFactor)?(new FunctionParser(*c.probabilityFactor) ):NULL;
    condition = (c.condition)?(new FunctionParser(*c.condition) ):NULL;
}

Rule & Rule::operator=( const Rule & c)
{
    this->strictPredecessor = c.strictPredecessor;
    this->variables = c.variables;
    this->leftContext = c.leftContext;
    this->rightContext = c.rightContext;
    probabilityFactor = (c.probabilityFactor)?(new FunctionParser(*c.probabilityFactor) ):NULL;
    condition = (c.condition)?(new FunctionParser(*c.condition) ):NULL;

    for(std::vector< StaticString *>::const_iterator it = c.staticStrings.begin(); it != c.staticStrings.end(); it++)
        staticStrings.push_back( new StaticString(**it) );

    for(std::vector< FunctionParser *>::const_iterator it = c.dynamicStrings.begin(); it != c.dynamicStrings.end(); it++)
        dynamicStrings.push_back( new FunctionParser(**it) );
    return *this;
}

Rule::~Rule()
{
    if(condition)
    {
        delete condition;
    }

    if(probabilityFactor)
    {
        delete probabilityFactor;
    }
}

void Rule::addQueryFunctions( FunctionParser * fp, string & expression )
{

    bool query = false;
#ifdef _MSC_VER
    if(expression.find( "positionX" ) != std::string::npos )
    {
        fp->AddFunction( "positionX", Query::positionX, 0);
        query = true;
    }
    if(expression.find( "positionY" ) != std::string::npos )
    {
        fp->AddFunction( "positionY", Query::positionY, 0);
        query = true;
    }
    if(expression.find( "positionZ" ) != std::string::npos )
    {
        fp->AddFunction( "positionZ", Query::positionZ, 0);
        query = true;
    }
	if(expression.find( "headingX" ) != std::string::npos )
	{
		fp->AddFunction( "headingX", Query::headingX, 0);
		query = true;
	}
	if(expression.find( "headingY" ) != std::string::npos )
	{
		fp->AddFunction( "headingY", Query::headingY, 0);
		query = true;
	}
	if(expression.find( "headingZ" ) != std::string::npos )
	{
		fp->AddFunction( "headingZ", Query::headingZ, 0);
		query = true;
	}
	if(expression.find( "upX" ) != std::string::npos )
	{
		fp->AddFunction( "upX", Query::upX, 0);
		query = true;
	}
	if(expression.find( "upY" ) != std::string::npos )
	{
		fp->AddFunction( "upY", Query::upY, 0);
		query = true;
	}
	if(expression.find( "upZ" ) != std::string::npos )
	{
		fp->AddFunction( "upZ", Query::upZ, 0);
		query = true;
	}
	if(expression.find( "leftX" ) != std::string::npos )
	{
		fp->AddFunction( "leftX", Query::leftX, 0);
		query = true;
	}
	if(expression.find( "leftY" ) != std::string::npos )
	{
		fp->AddFunction( "leftY", Query::leftY, 0);
		query = true;
	}
	if(expression.find( "leftZ" ) != std::string::npos )
	{
		fp->AddFunction( "leftZ", Query::leftZ, 0);
		query = true;
	}
#endif
    if(expression.find( "rand" ) != std::string::npos )
    {
        fp->AddFunction( "rand", Randomizer::get, 1);
        query = true;
    }

    if( !query )
    {
        //fp->Optimize();
    }

}

void Rule::processProbabilityFactor(string * rule, string::iterator & it)
{
    // 3 possibilities of empty probability factor
    //      - there is nothing after successor
    //      - there is something else then ':' which indicates probability factor
    //      - there is ':' followed by '*' which indicates no probability factor
    if((it == rule->end()) || (*it++ != ':'))
    {
        this->probabilityFactor = NULL;
        return;
    }
    else if(*it == '*')
    {
        this->probabilityFactor = NULL;
        return;
    }

    if(!probabilityFactor)
    {
        probabilityFactor = new FunctionParser();
    }

    string expression( it, rule->end() );

    this->addQueryFunctions( probabilityFactor, expression );

    if ( probabilityFactor->Parse( expression, this->variables, false ) != -1 )
    {
        throw ParsingException("Parsing of probability factor (" + expression + ") error!");
    }
	else
	{
/*		double p[4];
		for(int i = 0; i < 20; i++)
		{
			p[0] = (double)i;
			vrecko::logger.debugLog("Word: %f", probabilityFactor->Eval( p ) );
			
		}*/
	}
}

void Rule::processCondition(string * rule, string::iterator & it)
{
    unsigned int pos;

    if( *it != ':' )
    {
        this->condition = NULL;
        return;
    }

    if( *(++it) != '*' )
    {
        pos = rule->find( "->", it - rule->begin() );

        if ( pos == std::string::npos )
        {
            throw ParsingException("Symbol \'->\' wasn\'t found!");
        }
        else
        {
            if(!this->condition)
            {
                this->condition = new FunctionParser();

            }

            string expression( it, rule->begin() + pos );
            this->addQueryFunctions( condition, expression );

            if ( condition->Parse(expression , this->variables, false ) != -1 )
            {
                throw ParsingException("Parsing of condition expression error!");
            }

            it = rule->begin() + pos;
        }
    }
    else
    {
        it++;
        this->condition = NULL;
    }
}

void Rule::processParameters(string * rule, string::iterator & it)
{
    // parametric rule
    // example: (x,y)

    unsigned int pos;

    if( *it == '(' )
    {
        it++;

        pos = rule->find( ')', it - rule->begin() );
        if ( pos == std::string::npos )
        {
            throw ParsingException("Missing ending bracket!");
        }
        else
        {
            // if there is some variables, add coma sign before adding another portion
            if( !this->variables.empty() )
                this->variables.append( 1, ',' );

            this->variables.append( it, rule->begin() + pos);
        }

        it = rule->begin() + pos + 1;
    }
}

bool Rule::addStaticString(string * rule, string::iterator & begin)
{
    StaticString * pSS;
    LongString * ls = new LongString( 128 );
    unsigned int pos = begin - rule->begin();
    try
    {
        ls->convertFromString( rule, pos, ':');
    }
    catch(bad_lexical_cast&)
    {
        //Log::write( "static string - ex:" + ls->toString());
        begin = rule->begin() + pos/* + 1*/;
        pSS = new StaticString(ls);
        staticStrings.push_back(pSS);
        // not number - must be parsed
        return true;
    }
    //cout << "static string:" << ls->toString() << endl;

//        begin = rule->begin() + pos + 1;
    pSS = new StaticString(ls);
    staticStrings.push_back(pSS);
    return false;
}

void Rule::addDynamicString(string * rule, string::iterator & begin)
{
    unsigned int pos = begin - rule->begin() - 1, bracket = 1;

    while( bracket )
    {
        pos = rule->find_first_of( "(,)", pos+1 );
        switch(rule->at(pos))
        {
        case '(':
            bracket++;
            break;
        case ')':
            bracket--;
            break;
        case ',':
            if(bracket == 1)
                bracket = 0;
            break;
        default:
            throw ParsingException("parsing of expression error: parentheses matching error");
        }
    }

    std::string::iterator end = rule->begin() + pos;

    FunctionParser * fp = new FunctionParser( );

    string expression( begin, end );
    this->addQueryFunctions( fp, expression );

    // parse string inside brackets
    if ( fp->Parse( expression, this->variables, false ) != -1 )
    {
        throw ParsingException(string("parsing of expression error: ") + fp->ErrorMsg());
    }
    begin = end + 1;
    this->dynamicStrings.push_back( fp );
}

bool Rule::evaluateCondition( double * p )
{
    if( this->condition )
    {
        if( this->condition->Eval( p ) == 0 )
            return false;
        else
            return true;
    }
    else
        // no condition - always true
        return true;
}

