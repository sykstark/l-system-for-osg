#include "precompiled.h"
#include "defaultparser.h"

#include "configuration.h"

using namespace AP_LSystem;

DefaultParser::DefaultParser( osg::Group * owner )
{
	// TODO move to abstract
	if( owner )
	{
		pOwner = setShadows( owner );
		createGeodes();
	}
}

DefaultParser::~DefaultParser( )
{
	// TODO delete geodes

}

osg::Group * DefaultParser::setShadows( osg::Group * owner )
{
	osg::ref_ptr<osgShadow::ShadowedScene> shadowedScene = new osgShadow::ShadowedScene;
	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
    shadowedScene->setShadowTechnique(sm.get());
	owner->addChild( shadowedScene.get() );

	osg::Vec3 lightPosition(150,150,150); 
	osg::LightSource* ls = new osg::LightSource;
	ls->getLight()->setPosition(osg::Vec4(lightPosition,1));
	ls->getLight()->setAmbient(osg::Vec4(0.8,0.8,0.8,1.0));
	ls->getLight()->setDiffuse(osg::Vec4(1.0,1.0,1.0,1.0));

	shadowedScene->addChild(ls);

	lightPosition = osg::Vec3(-150,150,150); 
	ls = new osg::LightSource;
	ls->getLight()->setPosition(osg::Vec4(lightPosition,1));
	ls->getLight()->setAmbient(osg::Vec4(0.4,0.4,0.4,1.0));
	ls->getLight()->setDiffuse(osg::Vec4(0.7,0.7,0.7,1.0));

	owner->addChild(ls);

	lightPosition = osg::Vec3(100,100,0); 
	ls = new osg::LightSource;
	ls->getLight()->setPosition(osg::Vec4(lightPosition,0));
	ls->getLight()->setAmbient(osg::Vec4(0.4,0.4,0.4,1.0));
	ls->getLight()->setDiffuse(osg::Vec4(1.0,1.0,1.0,1.0));

	owner->addChild(ls);

	return shadowedScene;
}

int DefaultParser::parse(ParseableString * word)
{
	int res = 0;
	vector<Parameter> parameters;

	while(!word->eof( ))
	{

		// FREE SYMBOLS
		// ! " # % ' * , . : ; < > @ [ ] _ ` { } ? ~
		parameters.clear();
		switch( word->next( parameters ) )
		{
		case 'F':
			res = turtles.top()->drawForward( parameters );
			break;
		case 'Z':
			res = turtles.top()->drawForwardHalf( );
			break;
		case 'f':
			res = turtles.top()->moveForward( parameters );
			break;
		case 'z':
			res = turtles.top()->moveForwardHalf( );
			break;
		case '+':
			res = turtles.top()->turnLeft( parameters );
			break;
		case '-':
			res = turtles.top()->turnRight( parameters );
			break;
		case '&':
			res = turtles.top()->pitchDown( parameters );
			break;
		case '^':
			res = turtles.top()->pitchUp( parameters );
			break;
		case '\\':
			res = turtles.top()->rollLeft( parameters );
			break;
		case '/':
			res = turtles.top()->rollRight( parameters );
			break;
		case '|':
			res = turtles.top()->turnArround( );
			break;
		case '=':
			res = turtles.top()->rollUntilHorizontal( );
			break;
		case '\'':
			res = turtles.top()->multiplyLength( parameters );
			break;
		case '!':
			res = turtles.top()->multiplyRadius( parameters );
			break;
		case '$':
			res = switchGeode( parameters );
			break;
		case '[':
			turtles.push( );
			break;
		case ']':
			turtles.pop( );
			break;
		}	

		if ( res )
			return res;
	}
	return LS_OK;
}
