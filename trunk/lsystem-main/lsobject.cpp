#include "precompiled.h"

#include <exception>
#include "lsobject.h"
#include "configuration.h"
#include "turtleinterpret.h"
#include "lsystemgenerator.h"
#include "log.h"
#include "lsystemexception.h"

XERCES_CPP_NAMESPACE_USE

namespace AP_LSystem {

LSObject::LSObject() : Ability("LSObject") 
{
	pEOOwner = NULL;
}

LSObject::~LSObject() 
{
	pEOOwner = NULL;
}

void LSObject::preInitialize() 
{
	pEOOwner = (EnvironmentObject *)pOwner;
}

void LSObject::postInitialize() 
{
	// Schedule the parent EO for regular updates
	getWorldPtr()->getSchedulerPtr()->addEntity(this, 60);
	setShadows();

	ParseableString * pWord;	
	try
	{
		Configuration::get()->loadCfgFile( configFile );

		generator = new LSystemGenerator( );
		generator->loadFile( lsystemFile );
		pWord = generator->getWord();	
	}
	catch( std::exception & e)
	{
		logger.errorLog( "Exception: %s ", e.what() );
		//Log::write( e.what() );
		return;
	}

	AbstractInterpret * parser = new TurtleInterpret( /*LSScene*/pEOOwner );
	int res = parser->parse( pWord ); 
	if( res )
	{
		logger.errorLog( "Parser error: %d ", res );
		return;
	}

	return;
}

bool LSObject::loadXMLParameters(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *pParametersNode) 
{
	if (findXMLNode(pParametersNode, "LSystemFile")) {
		ReaderWriter::getStringValue(this->lsystemFile, pParametersNode, "LSystemFile");
	}
	else
	{
		//throw FileException("File with L-system is not set");
	}

	if (findXMLNode(pParametersNode, "ConfigFile")) {
		ReaderWriter::getStringValue(this->configFile, pParametersNode, "ConfigFile");
	}
	else
	{
		//throw FileException("File with configuration is not set");
	}
	return true;
}

void LSObject::setShadows( )
{
	return;

	LSScene = new osgShadow::ShadowedScene;
	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	//osg::ref_ptr<osgShadow::SoftShadowMap> ssm = new osgShadow::SoftShadowMap;
	LSScene->setShadowTechnique(sm.get());

	vrecko::World * world = getWorldPtr();
	
	/*
	vrecko::World * newWorld = new World();
	newWorld->addChild( LSScene.get() );
	LSScene->addChild( world );
	setWorldPtr( newWorld );
	*/
	pEOOwner->addChild( LSScene.get() );
	/**/

	osg::ref_ptr<osg::Geode> lightBalls = new osg::Geode;
	pEOOwner->addChild( lightBalls );
	osg::StateSet* state = /*newWorld*/world->getOrCreateStateSet();
	state->setMode( GL_LIGHTING, osg::StateAttribute::ON );
	state->setMode( GL_LIGHT0, osg::StateAttribute::ON );
	state->setMode( GL_LIGHT1, osg::StateAttribute::ON );
	//state->setMode( GL_LIGHT2, osg::StateAttribute::ON );

	osg::Vec3 lightPosition(50,50,50); 
	osg::LightSource* ls = new osg::LightSource;
	ls->getLight()->setPosition(osg::Vec4(lightPosition,1));
	ls->getLight()->setAmbient(osg::Vec4(0.3,0.3,0.3,1.0));
	ls->getLight()->setDiffuse(osg::Vec4(0.8,0.8,0.8,1.0));

	LSScene->addChild(ls);
	lightBalls->addDrawable( new osg::ShapeDrawable(new osg::Sphere( lightPosition, 2.0 ) ) );

	lightPosition = osg::Vec3(-50,50,50); 
	ls->getLight()->setLightNum(1);
	ls = new osg::LightSource;
	ls->getLight()->setPosition(osg::Vec4(lightPosition,1));
	ls->getLight()->setAmbient(osg::Vec4(0.2,0.2,0.2,1.0));
	ls->getLight()->setDiffuse(osg::Vec4(0.4,0.4,0.4,1.0));

	LSScene->addChild(ls);
	lightBalls->addDrawable( new osg::ShapeDrawable(new osg::Sphere( lightPosition, 2.0 ) ) );

	/*lightPosition = osg::Vec3(100,100,0); 
	ls = new osg::LightSource;
	ls->getLight()->setLightNum(2);
	ls->getLight()->setPosition(osg::Vec4(lightPosition,1));
	ls->getLight()->setAmbient(osg::Vec4(0.4,0.4,0.4,1.0));
	ls->getLight()->setDiffuse(osg::Vec4(1.0,1.0,1.0,1.0));*/

	//owner->addChild(ls);
}
}