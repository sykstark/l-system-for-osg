#include "precompiled.h"

#include "xmlfile.h"
#include "configuration.h"
#include "lsystemexception.h"
#include "boost/lexical_cast.hpp"
#include "utils.h"
#include <sys/stat.h>
#include <errno.h>

using namespace AP_LSystem;
using namespace xercesc;
using namespace vrecko;
using boost::lexical_cast;

XmlFile::XmlFile()
{
	try
	{
		XMLPlatformUtils::Initialize();  // Initialize Xerces infrastructure
	}
	catch( XMLException& e )
	{
		char * message = XMLString::transcode( e.getMessage() );
		std::string str = string("XML toolkit initialization error: ") + message;
		XMLString::release( &message );
		throw ParsingException( str );
		
		// throw exception here to return ERROR_XERCES_INIT
	}

	m_FileParser = new XercesDOMParser;
}

void XmlFile::open(std::string & filename)
{
	struct stat fileStatus;
	int iretStat = stat(filename.c_str(), &fileStatus);
	if( iretStat == ENOENT )
		throw FileException("Path file_name does not exist, or path is an empty string.");
	else if( iretStat == ENOTDIR )
		throw FileException("A component of the path is not a directory.");
	else if( iretStat == EACCES )
		throw FileException("Permission denied.");
	else if( iretStat == ENAMETOOLONG )
		throw FileException("File can not be read\n");

	m_FileParser->setValidationScheme( XercesDOMParser::Val_Never );
	m_FileParser->setDoNamespaces( false );
	m_FileParser->setDoSchema( false );
	m_FileParser->setLoadExternalDTD( false );

	try
	{
		m_FileParser->parse( filename.c_str() );

		// no need to free this pointer - owned by the parent parser object
		xercesc::DOMDocument* xmlDoc = m_FileParser->getDocument();

		// Get the top-level element: Name is "lsystem".
		DOMElement* elementRoot = xmlDoc->getDocumentElement();
		
		// is there a root ?
		if( !elementRoot ) 
			throw ParsingException( "empty XML document" );

		// Parse XML file for tags of interest
		
		// is the root LSystem ?
		std::string nodeName = XMLString::transcode(elementRoot->getTagName());
		if( nodeName != "LSystem" )
			throw ParsingException( "incorrect root node" );

		// set name of l-system
		if(findXMLNode( elementRoot, "Name", false ))
		{
			ReaderWriter::getStringValue(this->m_Name, elementRoot, "Name");
		}
		else
		{
			throw ParsingException( "\"Name\" node wasn't found. Unique name has to be set." );
		}

		// set axiom of l-system
		if(findXMLNode( elementRoot, "Axiom", false ))
		{
			ReaderWriter::getStringValue(this->m_Axiom, elementRoot, "Axiom");
		}
		else
		{
			throw ParsingException( "\"Axiom\" node wasn't found. Axiom is required." );
		}

		// process all parameters
		DOMNode * node = findXMLNode( elementRoot, "Parameters", false );
		if( node )
		{
			processParameters(node);
		}

		// parse type
		node = findXMLNode( elementRoot, "Types", false );
		if( node )
		{
			processType(node);
		}

		// parse rules
		node = findXMLNode( elementRoot, "Rules", false );
		if( node )
		{
			processRules(node);
		}

		// parse rules
		node = findXMLNode( elementRoot, "Homomorphisms", false );
		if( node )
		{
			processHomomorphisms(node);
		}

		// parse subsystems
		node = findXMLNode( elementRoot, "Subsystems", false );
		if( node )
		{
			processSubsystems(node);
		}

		// parse constants
		node = findXMLNode( elementRoot, "Constants", false );
		if( node )
		{
			processConstants(node);
			// substitute all occurences of defined macros by their values
			substitute( this->m_Defines );
		}
	}
	catch( xercesc::XMLException& e )
	{
		char* message = xercesc::XMLString::transcode( e.getMessage() );
		ostringstream errBuf;
		errBuf << "Error parsing file: " << message << flush;
		XMLString::release( &message );
	}
}

void XmlFile::processParameters(xercesc::DOMNode * parameters)
{
	DOMNodeList * children = parameters->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();
	string parameter, value;

	for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
	{
		DOMNode* currentNode = children->item(xx);
		if( currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
		{
			// Found node which is an Element. Re-cast node as element
			DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			Configuration::get()->setProperty(	this->m_Name, 
												XMLString::transcode(currentElement->getTagName()), 
												XMLString::transcode(currentElement->getTextContent()) );
		}
	}
}

void XmlFile::processRules(xercesc::DOMNode * subs)
{
	DOMNodeList * children = subs->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();
	string parameter, value;

	for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
	{
		DOMNode* currentNode = children->item(xx);
		if( currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
		{
			// Found node which is an Element. Re-cast node as element
			DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			if( string(XMLString::transcode(currentElement->getTagName())) == "Rule" )
				this->m_Rules.push_back(XMLString::transcode(currentElement->getTextContent()));
		}
	}
}

void XmlFile::processSubsystems(xercesc::DOMNode * subs)
{
	DOMNodeList * children = subs->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();
	string parameter, value;

	for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
	{
		DOMNode* currentNode = children->item(xx);
		//value = XMLString::transcode( currentNode->getNodeValue() );
		if( currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
		{
			// Found node which is an Element. Re-cast node as element
			DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			if( string(XMLString::transcode(currentElement->getTagName())) == "Subsystem" )
				this->m_Subsytems.push_back(StringUtils::eraseWhiteSpaces( std::string(XMLString::transcode(currentElement->getTextContent()))));
		}
	}
}

void XmlFile::processConstants(xercesc::DOMNode * node)
{
	DOMNodeList * children = node->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();
	string parameter, value;

	for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
	{
		DOMNode* currentNode = children->item(xx);
		if( currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
		{
			// Found node which is an Element. Re-cast node as element
			DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			m_Defines.insert(std::make_pair<std::string, std::string>(
				XMLString::transcode(currentElement->getTagName()),
				XMLString::transcode(currentElement->getTextContent() ) ) );
		}
	}
}

void XmlFile::processHomomorphisms(xercesc::DOMNode * subs)
{
	DOMNodeList * children = subs->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();
	string parameter, value;

	for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
	{
		DOMNode* currentNode = children->item(xx);
		if( currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
		{
			// Found node which is an Element. Re-cast node as element
			DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			if( string(XMLString::transcode(currentElement->getTagName())) == "Homomorphism" )
				this->m_Homomorphisms.push_back( StringUtils::eraseWhiteSpaces( string(XMLString::transcode(currentElement->getTextContent() )) ) );
		}
	}
}

void XmlFile::processType(xercesc::DOMNode * type)
{
	DOMNodeList * children = type->getChildNodes();
    const XMLSize_t nodeCount = children->getLength();
	string value;

	for( XMLSize_t xx = 0; xx < nodeCount; ++xx )
	{
		DOMNode* currentNode = children->item(xx);
		//value = XMLString::transcode( currentNode->getNodeValue() );
		if( currentNode->getNodeType() && currentNode->getNodeType() == DOMNode::ELEMENT_NODE ) // is element 
		{
			// Found node which is an Element. Re-cast node as element
			DOMElement* currentElement = dynamic_cast< xercesc::DOMElement* >( currentNode );

			if( string(XMLString::transcode(currentElement->getTagName())) == "Type" )
			{
				value = XMLString::transcode(currentElement->getTextContent());
				this->addType( StringUtils::eraseWhiteSpaces( value ) );
			}
		}
	}
}