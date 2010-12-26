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
		Randomizer::init();
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
			else
			{
				vrecko::logger.warningLog( "No parent node. LSGeode not added!" );
			}

			geodes.push_back( pGeode );
		}
		vrecko::logger.debugLog( "%d LSGeodes successfuly created and binded with scene graph.", count );
	}

	static const char * errorText( int error )
	{
		switch( error )
		{
		case LS_OK:
			return "OK. No error.";
		case LS_NOTDEFINED:
			return "Method is not defined";
		case LS_ERR_PAR_BADTYPE:
			return "Bad type of module parameter";
		case LS_ERR_PAR_INVALIDCOUNT:
			return "Invalid number of parameters";
		case LS_ERR_DRAWFORWARD_NEGATIVEDISTANCE:
			return "Negative value of step distance";
		case LS_ERR_STACK_UNKNOWN_TURTLE_TYPE:
			return "Unknown turtle type";
		case LS_ERR_STACK_NULL_LSGEODE:
			return "NULL geode";
		default:
			return "Unknown error code";
		}
	}

	virtual int parse( ParseableString * ) = 0;
};
}

#endif