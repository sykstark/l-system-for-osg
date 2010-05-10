#include "turtle.h"

class ContourTurtle: public Turtle
{
private:
	unsigned int drawColor, actualColor;
	bool autoRoll;

	vector<vmath::Vector4d> shapeVertices;
	vector<vmath::Vector4d> shapeNormals;

	vector<vmath::Vector4d> bottomCapVertices;
	vector<vmath::Vector4d> bottomCapNormals;
	vector<vmath::Vector4d> topCapVertices;
	vector<vmath::Vector4d> topCapNormals;

	bool forwardTriangulation();
	bool endTriangulation();
	virtual bool universalRotation(double angle, int axis);
	inline void createCircleContour(unsigned int detail)
	{
		if(detail<3) detail = 3;
		for(unsigned int i=0; i<detail; i++)
		{
			vmath::Vector3d v((_thickness/2.0)*cos((double)i*2.0*_PI/(double)detail),
							  (_thickness/2.0)*sin((double)i*2.0*_PI/(double)detail),
						      0.0);
			shapeVertices.push_back(vmath::Vector4d(v.x,v.y,v.z,1.0));
			v.normalize();
			shapeNormals.push_back(vmath::Vector4d(v.x,v.y,v.z,1.0));
		}
	};
public:
	ContourTurtle(double thickness, double angle, double length, unsigned int detail)
	{
		_thickness = thickness;
		_length = length;
		_angle = angle;
		_def_length = length;
		_def_angle = angle;
		_def_thickness = thickness;
		_color = 0;
		actualColor = 0;
		drawColor = 0;
		createCircleContour(detail);		
		loadColors();
	};
	ContourTurtle(Turtle* turtle, unsigned int detail)
	{
		_thickness = turtle->getThickness();
		_matrix = turtle->getMatrix();
		_rotMatrix = turtle->getRotationMatrix();
		_length = turtle->getLength();
		_angle = turtle->getAngle();
		_color = turtle->getColor();
		_def_length = turtle->getDefLength();
		_def_angle = turtle->getDefAngle();
		_def_thickness = turtle->getDefThickness();

		createCircleContour(detail);
		loadColors();
	};
	~ContourTurtle(void)
	{
	};
	ContourTurtle(const ContourTurtle &copy);
	ContourTurtle operator=(const ContourTurtle &copy);

	virtual bool initialize();
	virtual bool finalize();

	virtual bool forwardDrawMove(double length);

	virtual bool multiplyThickness(double multiplier);
	virtual bool resetValues();

	virtual inline void setColorIndex(unsigned int color){ _color = color; actualColor = color; };
	virtual inline void increaseColorIndex();

	inline vector<vmath::Vector4d> getShapeVertices() const { return shapeVertices;};
	inline vector<vmath::Vector4d> getShapeNormals() const { return shapeNormals;};
	inline vector<vmath::Vector4d> getBottomCapVertices() const { return bottomCapVertices;};
	inline vector<vmath::Vector4d> getBottomCapNormals() const { return bottomCapNormals;};
	inline vector<vmath::Vector4d> getTopCapVertices() const { return topCapVertices;};
	inline vector<vmath::Vector4d> getTopCapNormals() const { return topCapNormals;};
};
