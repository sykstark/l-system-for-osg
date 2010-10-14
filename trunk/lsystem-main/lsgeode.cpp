#include "precompiled.h"

#include "lsgeode.h"
#include "lsystemexception.h"
#include "Configuration.h"

using namespace AP_LSystem;
using namespace std;
using namespace boost::program_options;

LSGeode::LSGeode()
{
	std::string t = Configuration::get()->getProperty("turtle_type")->as<string>();
	setTurtleType(t);
}
/*
void LSGeode::configure()
{
	const string & name = this->getName();

	// configure geode options according to settings in Configuration
	
	Configuration::get()->getProperty( name, "");
}
*/
void LSGeode::setTurtleType(std::string & type)
{
	if(type == "JOINTED_PIPE" )
		turtleType = LS_TURTLE_JOINTEDPIPE;
	else if(type == "STRAIGHT_PIPE")
		turtleType = LS_TURTLE_JOINTEDPIPE;
	else
		throw ParsingException( "unknown turtle type" );
}

TurtleType LSGeode::getTurtleType()
{
	return turtleType;
}

TurtleProperties & LSGeode::getDefaultTurtleProperties( )
{
	return defaultTurtleProperties;
}

void LSGeode::setDefaultTurtleProperties( TurtleProperties & props )
{
	defaultTurtleProperties = props;
}



