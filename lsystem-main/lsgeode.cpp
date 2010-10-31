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
	else if(type == "RECTANGLE")
		turtleType = LS_TURTLE_RECTANGLE;
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
	osg::ref_ptr<osg::Vec2dArray> textures = new osg::Vec2dArray;
	p.geometry->setTexCoordArray( 0, textures.get() );
	// add geometry to LSGeode
	this->addDrawable( defaultTurtleProperties.geometry.get() );

	const variable_value * diffTexFile = Configuration::get()->getProperty( index , "diffuse_texture" );
	if( diffTexFile )
	{
		std::string file = diffTexFile->as<std::string>();
		osg::ref_ptr<osg::Image> diffIm = osgDB::readImageFile( file );
//		diffIm->setInternalTextureFormat(0);

//		bool i = diffIm->isImageTranslucent();

		osg::ref_ptr<osg::Texture2D> diffTexture = new osg::Texture2D;
		if(diffIm.get())
		{
			diffTexture->setImage( diffIm.get() );
			diffTexture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT ); // x-axis
			diffTexture->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT ); // y-axis

			osg::ref_ptr<osg::StateSet> state = this->getOrCreateStateSet();
/*
			state->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON);
			state->setAttributeAndModes( new osg::BlendFunc, osg::StateAttribute::ON );
			osg::AlphaFunc* alphaFunc = new osg::AlphaFunc;
			alphaFunc->setFunction(osg::AlphaFunc::GEQUAL,0.05f);
			state->setAttributeAndModes( alphaFunc, osg::StateAttribute::ON );
*/
			state->setTextureAttributeAndModes( 0, diffTexture.get(), osg::StateAttribute::ON );
			state->setMode(GL_BLEND,osg::StateAttribute::ON);
			state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);			
		}
	}
	// initialize starting T coordinate of texture
	p.texCoordT = 0.0f;

	const variable_value * diffMatFile = Configuration::get()->getProperty( index , "diffuse_material" );
	if( diffMatFile )
	{
/*		osg::ref_ptr<osg::StateSet> state = this->getOrCreateStateSet();
		state->setTextureAttributeAndModes( 0, diffTexture.get(), osg::StateAttribute::ON );
		state->setMode(GL_BLEND,osg::StateAttribute::ON);
		state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);	*/		
	}
	/* else if TODO material & color */

	// set anti-twist vector
	p.contourVec = LeftVec;
	p.lastFrame.makeIdentity();

	p.controlPoint = Center;

	// process flags
	p.flags = 0;
	if( Configuration::get()->getProperty(index, "minimize_twist")->as<unsigned int>() )
		p.flags |= TurtleProperties::MINIMIZE_TWIST;
	if( Configuration::get()->getProperty(index, "draw_debug_geometry")->as<unsigned int>() )
		p.flags |= TurtleProperties::DRAW_DEBUG_GEOMETRY;
	if( Configuration::get()->getProperty(index, "degrees_to_radians")->as<unsigned int>() )
		p.flags |= TurtleProperties::DEGREES_TO_RADIANS;

	p.texRepeatingS = Configuration::get()->getProperty( index, "texture_s_repeating")->as<unsigned int>();
	p.angle = Configuration::get()->getProperty( index, "default_angle" )->as<double>();
	p.length = Configuration::get()->getProperty( index, "default_length" )->as<double>();
	p.radius = Configuration::get()->getProperty( index, "default_radius" )->as<double>();
	p.lengthMultiplier = Configuration::get()->getProperty( index, "length_multiplier" )->as<double>();
	p.angleMultiplier = Configuration::get()->getProperty( index, "angle_multiplier" )->as<double>();
	p.radiusMultiplier = Configuration::get()->getProperty( index, "radius_multiplier" )->as<double>();
	p.contourDetail = Configuration::get()->getProperty( index, "contour_detail" )->as<unsigned int>();
	p.debugGeometryScale = Configuration::get()->getProperty( index, "debug_geometry_scale" )->as<double>();
	p.angleVariance = Configuration::get()->getProperty( index, "angle_variance" )->as<unsigned int>();

	if(p.flags & TurtleProperties::DEGREES_TO_RADIANS)
		p.angle = osg::DegreesToRadians( p.angle );
}


