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
	m_EOOwner = NULL;
}

LSObject::~LSObject() 
{
	m_EOOwner = NULL;
}

void LSObject::preInitialize() 
{
	m_EOOwner = (EnvironmentObject *)pOwner;
}

void LSObject::postInitialize() 
{
	// Schedule the parent EO for regular updates
	getWorldPtr()->getSchedulerPtr()->addEntity(this, 60);

	logger.debugLog( "LSystem started" );

	ParseableString * pWord;	
	try
	{
		Configuration::get()->loadCfgFile( m_ConfigFileName );

		m_Generator = new LSystemGenerator( );
		m_Generator->loadFile( m_LSystemFileName );
		pWord = m_Generator->getWord();
		logger.debugLog( "Word generated. Generator stats: Processed modules: %d, Rewritings: %d", 
			Log::get()->getModuleCounter(), Log::get()->getRewritingCounter() );

		if( Log::get()->getQueryCounter() )
			logger.debugLog( "Queries was processed. Number of queries: %d ", Log::get()->getQueryCounter() );
	}
	catch( std::exception & e)
	{
		logger.errorLog( "Exception: %s ", e.what() );
		//Log::write( e.what() );
		return;
	}

	m_Interpreter = new TurtleInterpret( m_EOOwner );
	int res = m_Interpreter->parse( pWord ); 
	if( res )
	{
		logger.errorLog( "Interpreter error: %s ", AbstractInterpret::errorText(res) );
		return;
	}
	else
	{
		logger.debugLog( "Geometry was successfuly generated. %d turtles processed.", Log::get()->getTurtleCounter() );	
	}

	return;
}

bool LSObject::loadXMLParameters(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *pParametersNode) 
{
	if (findXMLNode(pParametersNode, "LSystemFile")) {
		ReaderWriter::getStringValue(this->m_LSystemFileName, pParametersNode, "LSystemFile");
	}
	else
	{
		//throw FileException("File with L-system is not set");
	}

	if (findXMLNode(pParametersNode, "ConfigFile")) {
		ReaderWriter::getStringValue(this->m_ConfigFileName, pParametersNode, "ConfigFile");
	}
	else
	{
		//throw FileException("File with configuration is not set");
	}
	return true;
}
}