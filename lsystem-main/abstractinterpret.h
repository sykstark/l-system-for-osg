#ifndef ABSTRACTINTERPRET_H_
#define ABSTRACTINTERPRET_H_

#include <vector>

#include "configuration.h"
#include "lsgeode.h"
#include "parseablestring.h"
#include "turtlestack.h"

using std::vector;
using std::string;

namespace AP_LSystem {
class AbstractInterpret
{
protected:
	vector< LSGeode *> geodes;
	osg::Group * pOwner;
	TurtleStack turtles;
public:
	AbstractInterpret( )
	{
		pOwner = NULL;
	}

	int switchGeode( std::vector<Parameter> & p )
	{
		switch( p.size() )
		{
		case 0:
			turtles.pop();
			break;
		case 1:
			if (p[0].type != Parameter::LS_UBYTE)
				return LS_ERR_PAR_BADTYPE;	
			turtles.push( geodes[*(static_cast<unsigned char*>(p[0].value))] );
			break;
		default:
			return LS_ERR_PAR_INVALIDCOUNT;
		}

		return LS_OK;		
	}

	void createGeodes()
	{
		unsigned int count = Configuration::get()->getLSystemCount();
		LSGeode * pGeode;
		for( unsigned int i = 0; i < count; i++)
		{
			pGeode = new LSGeode( i );

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