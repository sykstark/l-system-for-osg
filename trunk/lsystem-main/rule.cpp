#include "rule.h"

#include "boost/lexical_cast.hpp"
#include "lsystemexception.h"
#include "longstring.h"
#include "log.h"
#ifdef _MSC_VER
#include "query.h"
#endif

using boost::lexical_cast;
using boost::bad_lexical_cast;
using namespace AP_LSystem;

Rule::Rule( const Rule & c )
{
    this->variables = c.variables;
    this->leftContext = c.leftContext;
    this->rightContext = c.rightContext;
    this->condition = c.condition;
    this->probabilityFactor = c.probabilityFactor;

    for(unsigned int i = 0; i < c.staticStrings.size(); i++)
    {
        this->staticStrings.push_back(c.staticStrings.at(i));
    }

    for(unsigned int i = 0; i < c.dynamicStrings.size(); i++)
    {
        this->dynamicStrings.push_back(c.dynamicStrings.at(i));
    }
}

Rule & Rule::operator=( const Rule & c)
{
    this->variables = c.variables;
    this->leftContext = c.leftContext;
    this->rightContext = c.rightContext;
    this->condition = c.condition;
    this->probabilityFactor = c.probabilityFactor;

    for(unsigned int i = 0; i < c.staticStrings.size(); i++)
    {
        this->staticStrings.push_back(c.staticStrings.at(i));
    }

    for(unsigned int i = 0; i < c.dynamicStrings.size(); i++)
    {
        this->dynamicStrings.push_back(c.dynamicStrings.at(i));
    }

    return *this;
}

Rule::~Rule()
{
    if(condition)
    {
        //delete condition;
    }

    if(probabilityFactor)
    {
        //delete probabilityFactor;
    }
}

void Rule::addQueryFunctions( FunctionParser * fp, string & expression )
{
#ifdef _MSC_VER
    bool query = false;
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

    if( !query )
    {
        // TODO optimalize expression
    }
#endif
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

