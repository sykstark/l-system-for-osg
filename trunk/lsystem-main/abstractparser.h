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

			initializeTurtleProperties( pGeode->getDefaultTurtleProperties(), i );

			geodes.push_back( pGeode );
		}
	}

	void initializeTurtleProperties( TurtleProperties & prop, unsigned int index )
	{
		prop.length = Configuration::get()->getProperty( index, "default_length" )->as<double>();
		prop.angle = Configuration::get()->getProperty( index, "default_angle" )->as<double>();
		prop.radius = Configuration::get()->getProperty( index, "default_radius" )->as<double>();
		prop.lengthIncrement = Configuration::get()->getProperty( index, "length_increment" )->as<double>();
		prop.contourDetail = Configuration::get()->getProperty( index, "contour_detail" )->as<unsigned int>();
	}

	virtual int parse( ParseableString * ) = 0;
};
}

#endif