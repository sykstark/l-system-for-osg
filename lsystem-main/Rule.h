//#pragma once

#ifndef RULE_H_
#define RULE_H_

#include <iostream>
#include <vector>
#include "fparser/fparser.hh"
#include "boost/lexical_cast.hpp"
#include "lsystemexception.h"
#include "StaticString.h"
#include "longstring.h"

using namespace std;
using boost::lexical_cast;
using boost::bad_lexical_cast;

namespace AP_LSystem {
struct Rule
{
	FunctionParser* condition;
	std::string variables;
    FunctionParser * probabilityFactor;
    std::vector<StaticString*> staticStrings;
	std::vector<FunctionParser*> dynamicStrings;

    Rule():condition(NULL),variables(""),probabilityFactor(NULL){}

    Rule(const Rule & c)
    {
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

    Rule& operator=( const Rule & c)
    {
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

    ~Rule()
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

    void processProbabilityFactor(string * rule, string::iterator & it)
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

        FunctionParser * pf = this->probabilityFactor;
        if(!pf)
        {
            pf = new FunctionParser();
        }

        if ( pf->Parse( string( it, rule->end() ), this->variables, false ) != -1 )
        {
            throw ParsingException("Parsing of probability factor error!");
        }
    }

    void processCondition(string * rule, string::iterator & it)
    {
        unsigned int pos;

        if( *it++ != ':' )
        {
            if( (*it - 1 != '-') || (*it != '>') )
            {
                throw ParsingException("Symbol \':\' or \'->\' was expected!");
            }
            else
            {
                this->condition = NULL;
                return;
            }
        }

        if( *it != '*' )
        {
            pos = rule->find( "->", it - rule->begin() );

            if ( pos == std::string::npos )
            {
                throw ParsingException("Symbol \'->\' wasn\'t found!");
            }
            else
            {
                FunctionParser * cond = this->condition;
                if(!cond)
                {
                    cond = new FunctionParser();
                }

                if ( cond->Parse( string( it, rule->begin() + pos ), this->variables, false ) != -1 )
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

    void processParameters(string * rule, string::iterator & it)
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
                this->variables = string( it, rule->begin() + pos);
            }

            it = rule->begin() + pos + 1;
        }
    }

    bool addStaticString(string * rule, string::iterator & begin)
    {
        StaticString * pSS;
        LongString * ls = new LongString( 128 );
        unsigned int pos = begin - rule->begin();
        try
        {            
            ls->convertFromString( rule, pos);
        }
        catch(bad_lexical_cast&)
        {
            cout << "static string - ex:" << ls->toString() << endl;
            begin = rule->begin() + pos + 1;
            pSS = new StaticString(ls);
            staticStrings.push_back(pSS);
            // not number - must be parsed
            return true;
        }
        cout << "static string:" << ls->toString() << endl;

        begin = rule->begin() + pos + 1;
        pSS = new StaticString(ls);
        staticStrings.push_back(pSS);
        return false;

        /*std::string::iterator end;
        unsigned int pos;
        double par;
        LongString ls(512);
        StaticString * pSS;
        while(true)
        {
            pos = rule->find_first_of( "(:,)", begin - rule->begin( ) );
            if ( pos == std::string::npos )
            {
                ls.appendStr(string(begin,rule->end()));
                pSS = new StaticString(ls);
                // if(ls.length())
                staticStrings.push_back(pSS);
                return false;
            }
            end = rule->begin() + pos;
            std::string str(begin, end);

            switch(*end)
            {
            case ':':
                ls.appendStr(str);
                pSS = new StaticString(ls);
                staticStrings.push_back(pSS);
                return false;
            case ',':
            case ')':
                try
                {
                    par = lexical_cast<double>(str);
                }
                catch(bad_lexical_cast&)
                {
                    pSS = new StaticString(ls);
                    staticStrings.push_back(pSS);
                    // not number - must be parsed
                    return true;
                }
                ls.appendDouble(par);
                break;
            case '(':
                ls.appendStr(str);
                break;
            default:
                throw ParsingException("Successor parsing error");
            }
            begin = end+1;
        }
        */
    }

    void addDynamicString(string * rule, string::iterator & begin)
    {
        unsigned int pos = rule->find_first_of( ",)", begin - rule->begin() );
        std::string::iterator end = rule->begin() + pos;

        FunctionParser * fp = new FunctionParser( );

        cout << string( begin, end ) << endl;
        // parse string inside brackets
        if ( fp->Parse( string( begin, end ), this->variables, false ) != -1 )
        {
            throw ParsingException("parsing of expression error");
        }
        begin = end + 1;
        this->dynamicStrings.push_back( fp );
    }

protected:
    std::vector<StaticString*> getSS() const { return staticStrings; }
    std::vector<FunctionParser*> getDS() const { return dynamicStrings; }

};
}

#endif
