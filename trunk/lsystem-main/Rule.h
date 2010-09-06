#pragma once

#ifndef FPARSER
#define FPARSER
#include "fparser/fparser.hh"
#endif

#include "lsystemexception.h"
#include <vector>
#include "StaticString.h"

#include <iostream>

using namespace std;

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

protected:
    std::vector<StaticString*> getSS() const { return staticStrings; }
    std::vector<FunctionParser*> getDS() const { return dynamicStrings; }

};
}
