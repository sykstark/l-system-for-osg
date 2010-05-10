#include "turtle.h"

class PolygonTurtle: public Turtle
{
private:
	virtual bool universalRotation(double angle, int axis);
public:
	PolygonTurtle(double angle, double length, unsigned int detail)
	{
		_length = length;
		_angle = angle;
		_def_length = length;
		_def_angle = angle;
		_color = 0;		
		loadColors();
	};
	PolygonTurtle(Turtle* turtle)
	{
		_matrix = turtle->getMatrix();
		_rotMatrix = turtle->getRotationMatrix();
		_length = turtle->getLength();
		_angle = turtle->getAngle();
		_def_length = turtle->getDefLength();
		_def_angle = turtle->getDefAngle();
		_color = turtle->getColor();
		loadColors();
	};
	~PolygonTurtle(void){};
	PolygonTurtle(const PolygonTurtle &copy);
	PolygonTurtle operator=(const PolygonTurtle &copy);

	virtual bool initialize();
	virtual bool finalize();

	virtual bool forwardDrawMove(double length);

	virtual bool resetValues();

	virtual bool multiplyThickness(double multiplier){return true;};
	virtual inline void setColorIndex(unsigned int color){_color = color;};
	virtual inline void increaseColorIndex();
};
