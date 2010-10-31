#ifndef LSGEODE_H_
#define LSGEODE_H_

namespace AP_LSystem {
/**
 *	Struct for storing properties of turtles
 */
struct TurtleProperties
{
	enum Flags
	{
		DRAW_DEBUG_GEOMETRY		= 0x00000001,
		MINIMIZE_TWIST			= 0x00000002,
		DEGREES_TO_RADIANS		= 0x00000004,

	};

	TurtleProperties():modelFile(NULL){};

	/**
	 * Destructor of all objects in properties structure. This function is called in AbstractTurtle destructor.
	 */
	void clean()
	{
		if(modelFile)
			delete modelFile;
	}

	osg::ref_ptr<osg::Geometry> geometry;	///< pointer for quick access to geode's geometry

	osg::Matrixd matrix;	///< matrix of turtle position and orientation

	osg::Matrixd lastFrame;

	double length;					///< default length of one step
	double lengthMultiplier;		///< default length multiplier
	double angle;					///< default angle of rotation
	double angleMultiplier;			///< default angle multiplier
	unsigned int angleVariance;		///< variance of angle - in percents
	double radius;					///< default radius of each segment
	double radiusMultiplier;		///< default radius multiplier
	double texCoordT;				///< current texture coordinate in T direction
	unsigned int texRepeatingS;		///< number of repeatings of texture arround contour - S direction

	unsigned int curveDetail;	///< detail of generalized pipe curves

	osg::Vec3 controlPoint;		///< coordinates of center point of each move. Used for curve generating and for texturing.

	unsigned int flags;

	std::string * modelFile;	///< filename of model file - only for turtles that load models

	osg::ref_ptr<osg::Vec3dArray> contour;		///< contour vertices
	osg::ref_ptr<osg::Vec3dArray> contourLastV;	///< used for loft turtles - predecessing contour vertices
	osg::ref_ptr<osg::Vec3dArray> contourLastN;	///< used for loft turtles - predecessing contour normals
	unsigned int contourDetail;					///< detail of circle contour
	osg::Vec3d contourVec;						///< vector of first vertex of contour, used for minimalizing twist

	vector<osg::ref_ptr<osg::Vec3dArray> > hemisphere;

	double debugGeometryScale;		///< scale of debug geometry
};
/**
 * Turtle types
 */
enum TurtleType
{
	LS_TURTLE_JOINTEDPIPE,
	LS_TURTLE_STRAIGHTPIPE,
	LS_TURTLE_HERMITPIPE,
	LS_TURTLE_OBJMODEL,
	LS_TURTLE_RECTANGLE,
};

/**
 * OSG geometric node with added 
 *  - capability to configure node properties from Configuration
 *  - storing of turtle type that will be used to generate geometry
 */
class LSGeode :	public osg::Geode
{
private:
	TurtleType turtleType;
	TurtleProperties defaultTurtleProperties;
public:
	void setTurtleType(std::string &);
	TurtleType getTurtleType();
	void setDefaultTurtleProperties( TurtleProperties & );
	void setDefaultTurtleProperties( int );
	TurtleProperties & getDefaultTurtleProperties( );

	LSGeode( unsigned int );
	void configure();
};
}

#endif