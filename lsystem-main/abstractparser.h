#ifndef ABSTRACTPARSER_H_
#define ABSTRACTPARSER_H_

#include <vector>

#include "Configuration.h"
#include "lsgeode.h"
//#include "longstring.h"
#include "parseablestring.h"
#include "turtlestack.h"

using std::vector;
using std::string;

namespace AP_LSystem {
class AbstractParser
{
private:
	vector< LSGeode *> geodes;
protected:
	osg::Group * pOwner;
	LSGeode * selectedGeode;
	TurtleStack turtles;
public:
	AbstractParser( )
	{
		pOwner = NULL;
		selectedGeode = NULL;
//		createGeodes( );
	}

	int switchGeode( std::vector<Parameter> & p )
	{
		switch( p.size() )
		{
		case 0:
			turtles.pop();
			break;
		case 1:
			if (p[0].type != LS_UBYTE)
				return LS_ERR_PAR_BADTYPE;	
			selectedGeode = geodes[*(static_cast<unsigned char*>(p[0].value))];
			turtles.replace( selectedGeode );
			break;
		default:
			return LS_ERR_PAR_INVALIDCOUNT;
		}

		return LS_OK;		
	}

	void createGeodes()
	{
		unsigned int count = Configuration::get()->getGrammarCount();
		LSGeode * pGeode;
		for( unsigned int i = 0; i < count; i++)
		{
			pGeode = new LSGeode( i );

			// TODO configuration of Geode
			//pGeode->setSOMETHING( Configuration::get()->getProperty( i, "something")->as<double>() );
			if( pOwner )
			{
				pOwner->addChild( (osg::Group *)pGeode );
			}

			geodes.push_back( pGeode );
		}
	}

	virtual int parse( ParseableString * ) = 0;
};
}

#endif