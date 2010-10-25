#ifndef LSGEODE_H_
#define LSGEODE_H_

namespace AP_LSystem {
/**
 *	Struct for storing properties of turtles
 */
struct TurtleProperties
{
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

	double length;			///< default length of one step
	double lengthIncrement;	///< default length increment
	double angle;			///< default angle of rotation
	double angleIncrement;	///< default angle increment
	double radius;				///< default radius of each segment
	double radiusIncrement;		///< default radius increment

	unsigned int curveDetail;	///< detail of generalized pipe curves

	unsigned int flags;

	std::string * modelFile;	///< filename of model file - only for turtles that load models

	osg::ref_ptr<osg::Vec3dArray> contour;		///< contour vertices
	osg::ref_ptr<osg::Vec3dArray> contourLastV;	///< used for loft turtles - predecessing contour vertices
	osg::ref_ptr<osg::Vec3dArray> contourLastN;	///< used for loft turtles - predecessing contour normals
	unsigned int contourDetail;					///< detail of circle contour
	osg::Vec3d contourVec;						///< vector of first vertex of contour, used for minimalizing twist

	vector<osg::ref_ptr<osg::Vec3dArray> > hemisphere;
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