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
		prop.thickness = Configuration::get()->getProperty( index, "default_thickness" )->as<double>();
	}

	virtual int parse( ParseableString * ) = 0;
};
}

#endif