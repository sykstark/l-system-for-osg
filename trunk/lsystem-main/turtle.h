#pragma once

#ifndef _PI
#define _PI           3.14159265358979323846  /* pi */
#endif

#ifndef TO_RAD
#define TO_RAD        0.01745329251994329576
#endif

#ifndef POINTS
#define POINTS 0
#endif

#ifndef TRIANGLE_STRIP
#define TRIANGLE_STRIP 5
#endif

#ifndef TRIANGLE_FAN
#define TRIANGLE_FAN 6
#endif

#ifndef LINE_STRIP
#define LINE_STRIP 3
#endif

class Indices;

class Turtle
{
protected:
	vmath::Matrix4d _matrix, _rotMatrix;
	double _angle, _length, _thickness, _def_angle, _def_length, _def_thickness;
	unsigned int _color;

	vector<vmath::Vector4d> colors;

	bool vertexTriangulation();
	virtual bool universalRotation(double angle, int axis)=0;
	
	vector<vmath::Vector3d> verticesOutput;
	vector<vmath::Vector3d> normalsOutput;
	vector<Indices> indicesOutput;

	inline void loadColors()
	{
		colors.push_back(vmath::Vector4d(0.4,0.18,0.076078,1.0));
		colors.push_back(vmath::Vector4d(0.5,0.5,0.5,1.0));
		colors.push_back(vmath::Vector4d(0.0,0.9,0.0,1.0));
		colors.push_back(vmath::Vector4d(1.0,1.0,0.0,1.0)); 
		colors.push_back(vmath::Vector4d(0.2,0.7,0.1,1.0));
		colors.push_back(vmath::Vector4d(0.0,1.0,1.0,1.0));
		colors.push_back(vmath::Vector4d(0.0,0.0,1.0,1.0));
		colors.push_back(vmath::Vector4d(1.0,0.0,1.0,1.0));
		colors.push_back(vmath::Vector4d(0.439216,0.858824,0.576471,1.0)); 
		colors.push_back(vmath::Vector4d(1.0,0.498039,0.0,1.0));
		colors.push_back(vmath::Vector4d(0.258824,0.258824,0.435294,1.0)); 
		colors.push_back(vmath::Vector4d(0.6,0.196078,0.8,1.0));
		colors.push_back(vmath::Vector4d(0.439216,0.576471,0.858824,1.0)); 
		colors.push_back(vmath::Vector4d(0.556863,0.137255,0.137255,1.0));
		colors.push_back(vmath::Vector4d(0.858824,0.858824,0.439216,1.0));
		colors.push_back(vmath::Vector4d(0.623529,0.623529,0.372549,1.0));
	};


public:
	~Turtle(void);
	
	virtual bool initialize()=0;
	virtual bool finalize()=0;

	bool turnLeft();
	bool turnLeft(double angle);
	bool turnRight();
	bool turnRight(double angle);
	bool pitchDown();
	bool pitchDown(double angle);
	bool pitchUp();
	bool pitchUp(double angle);
	bool rollLeft();
	bool rollLeft(double angle);
	bool rollRight();
	bool rollRight(double angle);

	bool turnArround();
	bool rollArround();
	bool rollUntilHorizontal();
	bool randomTurnPitchRoll();
	bool randomTurnPitchRoll(unsigned int angle);
	bool gravityCorrection(double ratio);

	virtual bool resetValues()=0;

	bool fullForwardDrawMove();
	bool halfForwardDrawMove();
	virtual bool forwardDrawMove(double length)=0;

	bool fullForwardMove();
	bool forwardMove(double length);

	vector<vmath::Vector4d> getPosition();

	bool multiplyLength(double multiplier);
	bool multiplyAngle(double multiplier);
	virtual bool multiplyThickness(double multiplier)=0;

	inline vmath::Matrix4d getMatrix() const { return _matrix;};
	inline vmath::Matrix4d getRotationMatrix() const { return _rotMatrix;};

	inline double getThickness() const { return _thickness;};
	inline double getAngle() const { return _angle;};
	inline double getLength() const { return _length;};

	inline double getDefThickness() const { return _def_thickness;};
	inline double getDefAngle() const { return _def_angle;};
	inline double getDefLength() const { return _def_length;};

	inline unsigned int getColor() const { return _color;};
	inline vector<vmath::Vector4d> getColors() const { return colors;};
	inline vector<vmath::Vector3d> getVertices() const { return verticesOutput;};
	inline vector<vmath::Vector3d> getNormals() const { return normalsOutput;};
	inline vector<Indices> getIndices() const { return indicesOutput;};
	
	virtual inline void setColorIndex(unsigned int color)=0;
	virtual inline void increaseColorIndex()=0;
};

class Indices
{
public:
	Indices(unsigned int primitiveSet,unsigned int count, vmath::Vector4d color): primitiveSet(primitiveSet),count(count), color(color){};
	unsigned int primitiveSet, count;
	vmath::Vector4d color;
};