#pragma once

#ifndef FPARSER
#define FPARSER
#include "fparser/fparser.hh"
#endif

#include <vector>
#include "StaticString.h"

#include <iostream>

using namespace std;

namespace AP_LSystem {
//struct StaticString;
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

protected:
    std::vector<StaticString*> getSS() const { return staticStrings; }
    std::vector<FunctionParser*> getDS() const { return dynamicStrings; }

};
}
