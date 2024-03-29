#include "precompiled.h"

#include "lsgeode.h"
#include "lsystemexception.h"
#include "configuration.h"
#include "abstractturtle.h"

using namespace AP_LSystem;

using namespace boost::program_options;

LSGeode::LSGeode( unsigned int index )
{
	std::string t = Configuration::get()->getProperty( index , "TurtleType")->as<string>();
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
	else if(type == "HERMIT_PIPE")
		turtleType = LS_TURTLE_HERMITPIPE;
	else if(type == "RECTANGLE")
		turtleType = LS_TURTLE_RECTANGLE;
	else
		throw ParsingException( "unknown turtle type" );
}

void LSGeode::setTurtleType(TurtleType type)
{
	turtleType = type;
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
	// vector values
	std::vector<double> values;

	// set reference
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
	colors->push_back( White );
	
	// create normal array
	osg::ref_ptr<osg::Vec3dArray> normals = new osg::Vec3dArray;
	p.geometry->setNormalArray( normals.get() );
	p.geometry->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
	
	// create textures array
	osg::ref_ptr<osg::Vec2dArray> textures = new osg::Vec2dArray;
	p.geometry->setTexCoordArray( 0, textures.get() );
	
	// add geometry to LSGeode
	this->addDrawable( defaultTurtleProperties.geometry.get() );

	osg::ref_ptr<osg::StateSet> state = this->getOrCreateStateSet();

	const variable_value * diffTexFile = Configuration::get()->getProperty( index , "DiffuseTexture" );
	if( diffTexFile )
	{
		std::string file = diffTexFile->as<std::string>();
		osg::ref_ptr<osg::Image> diffIm = osgDB::readImageFile( file );

		osg::ref_ptr<osg::Texture2D> diffTexture = new osg::Texture2D;
		if(diffIm.get())
		{
			diffTexture->setImage( diffIm.get() );
			diffTexture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT ); // x-axis
			diffTexture->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT ); // y-axis
			
			state->setTextureAttributeAndModes( 0, diffTexture.get(), osg::StateAttribute::ON );		

			// for translucent textures
			if ( diffIm->isImageTranslucent() )
			{
				// set blending 
				state->setMode(GL_BLEND,osg::StateAttribute::ON);
				state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
   
				// use lighting for both sides     
				osg::ref_ptr<osg::LightModel> lm = new osg::LightModel;
  				lm->setTwoSided(true);
  				state->setAttribute( lm.get(), osg::StateAttribute::OVERRIDE );			
			}
		}
		else
		{
			vrecko::logger.warningLog( "Texture %s not laoded !", file.c_str() );
		}
	}

	const variable_value * mat = Configuration::get()->getProperty( index , "DiffuseMaterial" );
	
	if( mat )
	{
		osg::Material* material = new osg::Material;
		// disable glColor
		material->setColorMode(osg::Material::OFF);
		
		// diffuse material
		StringUtils::processVector(Configuration::get()->getProperty( index, "DiffuseMaterial" )->as<std::string>(), values );
		material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(values[0],values[1],values[2],1.0f));
		values.clear();

		mat = Configuration::get()->getProperty( index , "AmbientMaterial" );
		if( mat )
		{
			StringUtils::processVector(mat->as<std::string>(), values );
			material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(values[0],values[1],values[2],1.0f));
			values.clear();
		}
		else
			material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0f,0.0f,0.0f,1.0f));

		mat = Configuration::get()->getProperty( index , "SpecularMaterial" );
		if( mat )
		{
			StringUtils::processVector(mat->as<std::string>(), values );
			material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(values[0],values[1],values[2],1.0f));
			values.clear();
		}
		else
			material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0f,0.0f,0.0f,1.0f));

		mat = Configuration::get()->getProperty( index , "EmissionMaterial" );
		if( mat )
		{
			StringUtils::processVector(mat->as<std::string>(), values );
			material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(values[0],values[1],values[2],1.0f));
			values.clear();
		}
		else
			material->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(0.0f,0.0f,0.0f,1.0f));
		
		state->setAttributeAndModes(material,osg::StateAttribute::ON);
		state->setMode(GL_LIGHTING,osg::StateAttribute::ON);
	}
	else
	{
		const variable_value * color = Configuration::get()->getProperty( index , "Color" );
		if( color )
		{
			StringUtils::processVector(color->as<std::string>(), values );
			colors->clear();
			colors->push_back( osg::Vec4d( values[0], values[1], values[2], 1.0 ) );
			values.clear();
		}
	}

	// initialize starting T coordinate of texture
	p.texCoordT = 0.0f;

	// set anti-twist frame
	p.lastFrame.makeIdentity();

	p.controlPoint = Center;

	// process flags
	p.flags = 0;
	if( Configuration::get()->getProperty(index, "MinimizeTwist")->as<unsigned int>() )
		p.flags |= TurtleProperties::MINIMIZE_TWIST;
	if( Configuration::get()->getProperty(index, "DrawDebugGeometry")->as<unsigned int>() )
		p.flags |= TurtleProperties::DRAW_DEBUG_GEOMETRY;
	if( Configuration::get()->getProperty(index, "DegreesToRadians")->as<unsigned int>() )
		p.flags |= TurtleProperties::DEGREES_TO_RADIANS;
	if( Configuration::get()->getProperty(index, "SeparateGeometryForTranslucent")->as<unsigned int>() )
		p.flags |= TurtleProperties::SEPARATE_GEOMETRY_FOR_TRANSLUCENT;
	if( Configuration::get()->getProperty(index, "DrawPipeCaps")->as<unsigned int>() )
		p.flags |= TurtleProperties::DRAW_PIPE_CAPS;
	if( Configuration::get()->getProperty(index, "UseQueries")->as<unsigned int>() )
		p.flags |= TurtleProperties::USE_QUERIES;

	p.texRepeatingS				= Configuration::get()->getProperty( index, "TextureSRepeatings")->as<unsigned int>();
	p.angle						= Configuration::get()->getProperty( index, "DefaultAngle" )->as<double>();
	p.length					= Configuration::get()->getProperty( index, "DefaultLength" )->as<double>();
	p.radius					= Configuration::get()->getProperty( index, "DefaultRadius" )->as<double>();
	p.lengthMultiplier			= Configuration::get()->getProperty( index, "LengthMultiplier" )->as<double>();
	p.angleMultiplier			= Configuration::get()->getProperty( index, "AngleMultiplier" )->as<double>();
	p.radiusMultiplier			= Configuration::get()->getProperty( index, "RadiusMultiplier" )->as<double>();
	p.contourDetail				= Configuration::get()->getProperty( index, "ContourDetail" )->as<unsigned int>();
	p.debugGeometryScale		= Configuration::get()->getProperty( index, "DebugGeometryScale" )->as<double>();
	p.angleVariance				= Configuration::get()->getProperty( index, "AngleVariance" )->as<unsigned int>();
	p.gravitropismElasticity	= Configuration::get()->getProperty( index, "GravitropismElasticity" )->as<double>();
	p.tropismElasticity			= Configuration::get()->getProperty( index, "TropismElasticity" )->as<double>();
	p.tropismAngle				= Configuration::get()->getProperty( index, "TropismAngle" )->as<double>();
	
	

	StringUtils::processVector(Configuration::get()->getProperty( index, "TropismVector" )->as<std::string>(), values );
	p.tropismVector = osg::Vec3d( values[0], values[1], values[2] );
	p.tropismVector.normalize();
	values.clear();

	// convert angles from degrees to radians
	if(p.flags & TurtleProperties::DEGREES_TO_RADIANS)
	{
		p.angle = osg::DegreesToRadians( p.angle );
		p.tropismAngle = osg::DegreesToRadians( p.tropismAngle );
	}
}


