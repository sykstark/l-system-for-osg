#ifndef QUERYINTERPRET_H_
#define QUERYINTERPRET_H_

#include "abstractinterpret.h"
#include "lsgeode.h"
#include "log.h"

namespace AP_LSystem {
class QueryInterpret : public AbstractInterpret
{
private:
	static QueryInterpret * interpret;			///< singleton instance
	osg::Matrixd matrix;						///< current turtle position matrix
	
	/**
	  * Bitmap to determine if an update is necessary. If query is for a bit that is already set/dirty, an update must be
	  * processed and all bits are cleared.
	  */
	unsigned dirty;

	LongString * word;							///< Pointer to word that will be parsed.

	/**
	  * Bit for every coordinate of all position vectors.
	  */
	enum dirtyBits
	{
		CLEAR			= 0x00000000,
		POSITION		= 0x0000000F,
		POSITIONX		= 0x00000001,
		POSITIONY		= 0x00000002,
		POSITIONZ		= 0x00000004,
		HEADING			= 0x000000F0,
		HEADINGX		= 0x00000010,
		HEADINGY		= 0x00000020,
		HEADINGZ		= 0x00000040,
		UP				= 0x00000F00,
		UPX				= 0x00000100,
		UPY				= 0x00000200,
		UPZ				= 0x00000400,
		LEFT			= 0x0000F000,
		LEFTX			= 0x00001000,
		LEFTY			= 0x00002000,
		LEFTZ			= 0x00004000,
		DIRTY			= 0xFFFFFFFF,
	};

	/**
	  * Private constructor
	  */
	QueryInterpret()
	{
		// create group for geodes
		m_Owner = new osg::Group();

		word = NULL;

		createGeodes();

		dirty = QueryInterpret::DIRTY;

		for( vector<LSGeode *>::iterator it = m_Geodes.begin(); it != m_Geodes.end(); it++ )
		{
			(*it)->setTurtleType( LS_TURTLE_QUERY );
		}
	};

	/**
	  * Updates current position matrix by parsing binded word and getting position matrix of last parsed turtle.
	  */
	void update()
	{
		ParseableString * pW = new ParseableString( word );
			
		if(!pW)
			return;

		parse( pW );
	}

	virtual int parse( ParseableString * word);
	
	~QueryInterpret(void);
	
public:
	/**
	  * Get singleton instance
	  */
	static QueryInterpret * get()
	{
		if(!interpret) interpret = new QueryInterpret();

		return interpret;
	}


	/**
	  * x coordinate of turtle position vector
	  */
	double positionX( )
	{
		if( dirty & QueryInterpret::POSITIONX )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::POSITION;
		}
		dirty |= QueryInterpret::POSITIONX;

		Log::get()->incQueryCounter();

		return matrix.getTrans().x();
	}

	/**
	  * y coordinate of turtle position vector
	  */
	double positionY( )
	{
		if( dirty & QueryInterpret::POSITIONY )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::POSITION;
		}
		dirty |= QueryInterpret::POSITIONY;

		Log::get()->incQueryCounter();

		return matrix.getTrans().y();
	}

	/**
	  * z coordinate of turtle position vector
	  */
	double positionZ( )
	{
		if( dirty & QueryInterpret::POSITIONZ )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::POSITION;
		}
		dirty |= QueryInterpret::POSITIONZ;

		Log::get()->incQueryCounter();

		return matrix.getTrans().z();
	}

	/**
	  * x coordinate of turtle heading vector
	  */
	double headingX( )
	{
		if( dirty & QueryInterpret::HEADINGX )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::HEADING;
		}
		dirty |= QueryInterpret::HEADING;

		osg::Vec3d v = matrix.getRotate() * HeadVec;

		Log::get()->incQueryCounter();

		return v.x();
	}

	/**
	  * y coordinate of turtle heading vector
	  */
	double headingY( )
	{
		if( dirty & QueryInterpret::HEADINGY )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::HEADING;
		}
		dirty |= QueryInterpret::HEADING;

		osg::Vec3d v = matrix.getRotate() * HeadVec;

		Log::get()->incQueryCounter();

		return v.y();
	}

	/**
	  * z coordinate of turtle heading vector
	  */
	double headingZ( )
	{
		if( dirty & QueryInterpret::HEADINGZ )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::HEADING;
		}
		dirty |= QueryInterpret::HEADING;

		osg::Vec3d v = matrix.getRotate() * HeadVec;

		Log::get()->incQueryCounter();

		return v.z();
	}

	/**
	  * x coordinate of turtle up vector
	  */
	double upX( )
	{
		if( dirty & QueryInterpret::UPX )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::UP;
		}
		dirty |= QueryInterpret::UP;

		osg::Vec3d v = matrix.getRotate() * UpVec;

		Log::get()->incQueryCounter();

		return v.x();
	}

	/**
	  * y coordinate of turtle up vector
	  */
	double upY( )
	{
		if( dirty & QueryInterpret::UPY )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::UP;
		}
		dirty |= QueryInterpret::UP;

		osg::Vec3d v = matrix.getRotate() * UpVec;

		Log::get()->incQueryCounter();

		return v.y();
	}

	/**
	  * z coordinate of turtle up vector
	  */
	double upZ( )
	{
		if( dirty & QueryInterpret::UPZ )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::UP;
		}
		dirty |= QueryInterpret::UP;

		osg::Vec3d v = matrix.getRotate() * UpVec;

		Log::get()->incQueryCounter();

		return v.z();
	}

	/**
	  * x coordinate of turtle left vector
	  */
	double leftX( )
	{
		if( dirty & QueryInterpret::LEFTX )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::LEFT;
		}
		dirty |= QueryInterpret::LEFT;

		osg::Vec3d v = matrix.getRotate() * LeftVec;

		Log::get()->incQueryCounter();

		return v.x();
	}

	/**
	  * y coordinate of turtle left vector
	  */
	double leftY( )
	{
		if( dirty & QueryInterpret::LEFTY )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::LEFT;
		}
		dirty |= QueryInterpret::LEFT;

		osg::Vec3d v = matrix.getRotate() * LeftVec;

		Log::get()->incQueryCounter();

		return v.y();
	}

	/**
	  * z coordinate of turtle left vector
	  */
	double leftZ( )
	{
		if( dirty & QueryInterpret::LEFTZ )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::LEFT;
		}
		dirty |= QueryInterpret::LEFT;

		osg::Vec3d v = matrix.getRotate() * LeftVec;

		Log::get()->incQueryCounter();

		return v.z();
	}

	/**
	  * Bind word with interpreter. Interpreter uses this word if update is needed (calling update() method).
	  */
	void bindWord( LongString * str)
	{
		word = str;
		matrix.makeIdentity();
	}
	
};
}

#endif

