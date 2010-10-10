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
		vector<string> names = Configuration::get()->getGrammarNames();
		vector<string>::iterator it;
		LSGeode * pGeode;
		for( it = names.begin(); it != names.end(); it++)
		{
			pGeode = new LSGeode();
			pGeode->setName( *it );
			pGeode->configure();
			geodes.push_back( pGeode );
		}
	}

	virtual int parse( ParseableString * ) = 0;
};
}

#endif