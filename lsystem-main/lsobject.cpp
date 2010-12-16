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

	AbstractInterpret * interpreter = new TurtleInterpret( /*LSScene*/pEOOwner );
	int res = interpreter->parse( pWord ); 
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
}