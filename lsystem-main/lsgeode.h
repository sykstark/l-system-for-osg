#ifndef LSGEODE_H_
#define LSGEODE_H_

namespace AP_LSystem {
/**
 *	Struct for storing properties of turtles
 */
struct TurtleProperties
{
	/**
	  * Boolean properties of turtle
	  */
	enum Flags
	{
		DRAW_DEBUG_GEOMETRY					= 0x00000001,		///< indicates if debug geometry will be displayed
		MINIMIZE_TWIST						= 0x00000002,		///< turns on the twist minimalization
		DEGREES_TO_RADIANS					= 0x00000004,		///< converts all angles to radians
		SEPARATE_GEOMETRY_FOR_TRANSLUCENT	= 0x00000008,		///< for each translucent geometry will be created one geometry. It removes Halo efect but it is power consuming.
		USE_QUERIES							= 0x00000010,		///< turns on the query processing
		DRAW_PIPE_CAPS						= 0x00000100,		///< draw caps at the end of cylinders ?
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

	osg::Matrixd matrix;			///< matrix of turtle position and orientation
	osg::Matrixd lastMatrix;		///< matrix of previous turtle position and orientation
	osg::Matrixd lastFrame;			///< matrix for storing not twisted frame for contour drawing

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

	// direction stimuli
	double elasticityMultiplier;		///< default multiplier of gravitropism elasticity and diatropism elasticity
	double gravitropismElasticity;		///< response of stems to gravity
	double tropismElasticity;			///< response to general tropism
	double tropismAngle;				///< angle of general tropism
	osg::Vec3d tropismVector;			///< direction of general tropism

	std::string * modelFile;	///< filename of model file - only for turtles that load models

	osg::ref_ptr<osg::Vec3dArray> contour;		///< contour vertices
	osg::ref_ptr<osg::Vec3dArray> contourLastV;	///< used for loft turtles - predecessing contour vertices
	osg::ref_ptr<osg::Vec3dArray> contourLastN;	///< used for loft turtles - predecessing contour normals
	unsigned int contourDetail;					///< detail of circle contour
	
	vector<osg::ref_ptr<osg::Vec3dArray> > hemisphere;	///< hemisphere template geometry

	double debugGeometryScale;		///< scale of debug geometry
};
/**
 * Turtle types
 */
enum TurtleType
{
	LS_TURTLE_JOINTEDPIPE,			///< cylinders with hemispheres
	LS_TURTLE_STRAIGHTPIPE,			///< straight cylinders
	LS_TURTLE_HERMITPIPE,			///< hermit curve cylinders - not implemented
	LS_TURTLE_OBJMODEL,				///< *.obj object loader - not implemented
	LS_TURTLE_RECTANGLE,			///< rectangles - effective for translucent leaves
	LS_TURTLE_QUERY,				///< specialized turtle for query processing
};

/**
 * OSG geometric node with added 
 *  - capability to configure node properties from Configuration
 *  - storing of turtle type that will be used to generate geometry
 */
class LSGeode :	public osg::Geode
{
private:
	TurtleType turtleType;						///< type of turtle that generates geometry into this LSGeode instance
	TurtleProperties defaultTurtleProperties;	///< default turtle properties
public:
	/**
	  * Set turtle type.
	  * @param type string is converted to turtle type and assigned to this LSGeode instance
	  */
	void setTurtleType(std::string & type);

	/**
	  * Set turtle type.
	  * @param type this type is assigned to this LSGeode instance
	  */
	void setTurtleType(TurtleType type);

	/**
	  * Set turtle type.
	  * @param type string is converted to turtle type and assigned to this LSGeode instance
	  */
	TurtleType getTurtleType();
	void setDefaultTurtleProperties( TurtleProperties & );
	void setDefaultTurtleProperties( int );
	TurtleProperties & getDefaultTurtleProperties( );

	LSGeode( unsigned int );
	void configure();
};
}

#endif