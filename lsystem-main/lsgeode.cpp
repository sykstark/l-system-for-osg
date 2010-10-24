#include "precompiled.h"

#include "lsgeode.h"
#include "lsystemexception.h"
#include "configuration.h"
#include "abstractturtle.h"

using namespace AP_LSystem;
using namespace std;
using namespace boost::program_options;

LSGeode::LSGeode( unsigned int index )
{
	std::string t = Configuration::get()->getProperty( index , "turtle_type")->as<string>();
	setTurtleType(t);
	setDefaultTurtleProperties( index );
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
		turtleType = LS_TURTLE_STRAIGHTPIPE;
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

void LSGeode::setDefaultTurtleProperties( int index )
{
	TurtleProperties & p = defaultTurtleProperties;
	// create geometry for turtle geometry
	p.geometry = new osg::Geometry();
	// create vertex array
	osg::ref_ptr<osg::Vec3dArray> vertices = new osg::Vec3dArray;
	p.geometry->setVertexArray( vertices.get() );
	// create colors array
	osg::ref_ptr<osg::Vec4dArray> colors = new osg::Vec4dArray;
	p.geometry->setColorArray( colors.get() );
	p.geometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	colors->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );
	// create normal array
	osg::ref_ptr<osg::Vec3dArray> normals = new osg::Vec3dArray;
	p.geometry->setNormalArray( normals.get() );
	p.geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
	// create textures array
	/*osg::ref_ptr<osg::Vec2Array> textures = new osg::Vec2Array;
	p.geometry->setTexCoordArray( 0, textures.get() );*/
	// add geometry to LSGeode
	this->addDrawable( defaultTurtleProperties.geometry.get() );

	// set anti-twist vector
	p.contourVec = LeftVec;

	p.length = Configuration::get()->getProperty( index, "default_length" )->as<double>();
	p.angle = Configuration::get()->getProperty( index, "default_angle" )->as<double>();
	p.radius = Configuration::get()->getProperty( index, "default_radius" )->as<double>();
	p.lengthIncrement = Configuration::get()->getProperty( index, "length_increment" )->as<double>();
	p.contourDetail = Configuration::get()->getProperty( index, "contour_detail" )->as<unsigned int>();
}


