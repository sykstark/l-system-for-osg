#ifndef ABSTRACTPARSER_H_
#define ABSTRACTPARSER_H_

#include <map>
#include <stack>

#include "lsgeode.h"
#include "longstring.h"

using std::vector;
using std::string;
using std::map;
using std::stack;

namespace AP_LSystem {
class AbstractParser
{
private:
	stack< AbstractTurtle * > turtles;
	map<string, LSGeode *> geodes;
	LSGeode * selectedGeode;
public:
	AbstractParser( )
	{
		selectedGeode = NULL;
	}

	LSGeode * switchGeode( string & name )
	{
		selectedGeode = geodes[name];
		return selectedGeode;
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
			geodes.insert( std::make_pair<string, LSGeode *>(*it, pGeode ) );
		}
	}

	virtual void parse( ParseableString * ) = 0;
	virtual void pushTurtle( );
	virtual void popTurtle( );
};
}

#endif