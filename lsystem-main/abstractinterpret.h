#ifndef ABSTRACTINTERPRET_H_
#define ABSTRACTINTERPRET_H_

#include <vector>

#include "configuration.h"
#include "lsgeode.h"
#include "parseablestring.h"
#include "turtlestack.h"

using std::vector;
using std::string;

namespace AP_LSystem {
class AbstractInterpret
{
protected:
	vector< LSGeode *> m_Geodes;
	osg::Group * m_Owner;
	TurtleStack m_Turtles;
public:
	AbstractInterpret( )
	{
		m_Owner = NULL;
		Randomizer::init();
	}

	/**
	  * This method is called if change of L-system is interpreted.
	  * @param p vector with parameters of module.
	  *				- 1 parameter: Begin of different L-system. Push new turtle of different L-system. The parameter is L-system ID
	  *				- 0 parameters: End of current L-system subword. Pop turtle from the stack.
	  */
	int switchGeode( std::vector<Parameter> & p )
	{
		switch( p.size() )
		{
		case 0:
			m_Turtles.pop();
			break;
		case 1:
			if (p[0].type != Parameter::LS_UBYTE)
				return LS_ERR_PAR_BADTYPE;	
			m_Turtles.push( m_Geodes[*(static_cast<unsigned char*>(p[0].value))] );
			break;
		default:
			return LS_ERR_PAR_INVALIDCOUNT;
		}

		return LS_OK;		
	}

	/**
	  * Initialize interpret. Scan for loaded L-systems and create LSGeodes instances for them.
	  */
	void createGeodes()
	{
		unsigned int count = Configuration::get()->getLSystemCount();
		LSGeode * pGeode;
		for( unsigned int i = 0; i < count; i++)
		{
			pGeode = new LSGeode( i );

			if( m_Owner )
			{
				m_Owner->addChild( (osg::Group *)pGeode );
			}
			else
			{
				vrecko::logger.warningLog( "No parent node. LSGeode not added!" );
			}

			m_Geodes.push_back( pGeode );
		}
		vrecko::logger.debugLog( "%d LSGeodes successfuly created and binded with scene graph.", count );
	}

	/**
	  * Translates error codes to error messages.
	  * @param error error code
	  * @return error message
	  */
	static const char * errorText( int error )
	{
		switch( error )
		{
		case LS_OK:
			return "OK. No error.";
		case LS_NOTIMPLEMENTED:
			return "Method is not defined";
		case LS_ERR_PAR_BADTYPE:
			return "Bad type of module parameter";
		case LS_ERR_PAR_INVALIDCOUNT:
			return "Invalid number of parameters";
		case LS_ERR_DRAWFORWARD_NEGATIVEDISTANCE:
			return "Negative value of step distance";
		case LS_ERR_STACK_UNKNOWN_TURTLE_TYPE:
			return "Unknown turtle type";
		case LS_ERR_STACK_NULL_LSGEODE:
			return "NULL geode";
		default:
			return "Unknown error code";
		}
	}

	/**
      * Interpretes a word by converting L-system modules to turtle commands.
      * @param word interpreted word
      * @return error code
      */
	virtual int parse( ParseableString * word ) = 0;
};
}

#endif