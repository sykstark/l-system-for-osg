#ifndef ABSTRACTPARSER_H_
#define ABSTRACTPARSER_H_

#include <vector>

#include "lsgeode.h"
#include "longstring.h"
#include "turtlestack.h"

using std::vector;
using std::string;

namespace AP_LSystem {
class AbstractParser
{
private:
	vector< LSGeode *> geodes;
protected:
	LSGeode * selectedGeode;
	TurtleStack turtles;
public:
	AbstractParser( )
	{
		selectedGeode = NULL;
	}

	void switchGeode( unsigned char i )
	{
		selectedGeode = geodes[i];
		turtles.replace( selectedGeode );
	}

	void createGeodes()
	{
		unsigned int count = Configuration::get()->getGrammarCount();
		LSGeode * pGeode;
		for( unsigned int i = 0; i < count; i++)
		{
			pGeode = new LSGeode();

			// TODO configuration of Geode
			//pGeode->setSOMETHING( Configuration::get()->getProperty( i, "something")->as<double>() );

			geodes.push_back( pGeode );
		}
	}

	virtual int parse( ParseableString * ) = 0;
};
}

#endif