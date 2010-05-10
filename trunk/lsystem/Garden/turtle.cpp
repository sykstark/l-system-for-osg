#include "precompiled.h"
#include "turtle.h"


Turtle::~Turtle(void)
{
/*	vector<vmath::Vector4d>::iterator v4It;
	vector<vmath::Vector3d>::iterator v3It;
	vector<Indices>::iterator inIt;*/
}



// ================ STANDARD TURTLE ROTATIONS ========================

// ----------- TURNING --------------

bool Turtle::turnLeft()
{
	return turnRight(-_angle);
}

bool Turtle::turnLeft(double angle)
{
	return turnRight(-angle);
}

bool Turtle::turnRight()
{
	return turnRight(_angle);
}

bool Turtle::turnRight(double angle)
{
	return universalRotation(angle,0);
}

bool Turtle::turnArround()
{
	return turnRight(180.0);
}

// ----------- ROLLING --------------

bool Turtle::rollLeft()
{
	return rollRight(-_angle);
}

bool Turtle::rollLeft(double angle)
{
	return rollRight(-angle);
}

bool Turtle::rollRight()
{
	return rollRight(_angle);
}

bool Turtle::rollRight(double angle)
{
	return universalRotation(angle,2);
}

bool Turtle::rollArround()
{
	return rollRight(180.0);
}
// ----------- PITCHING --------------

bool Turtle::pitchDown()
{
	return pitchUp(-_angle);
}

bool Turtle::pitchDown(double angle)
{
	return pitchUp(-angle);
}

bool Turtle::pitchUp()
{
	return pitchUp(_angle);
}

bool Turtle::pitchUp(double angle)
{
	return universalRotation(angle,1);
}

bool Turtle::rollUntilHorizontal()
{
	vmath::Vector3d h(_matrix.at(2,0),_matrix.at(2,1),_matrix.at(2,2)), l, u;
	l = h.crossProduct(vmath::Vector3d(0.0,0.0,1.0));
	l /= l.length();
	u = h.crossProduct(l);
	_matrix.at(1,0) = l.x ; _matrix.at(1,1) = l.y ; _matrix.at(1,2) = l.z ;
	_matrix.at(0,0) = u.x ; _matrix.at(0,1) = u.y ; _matrix.at(0,2) = u.z ;

	_rotMatrix.at(1,0) = l.x ; _rotMatrix.at(1,1) = l.y ; _rotMatrix.at(1,2) = l.z ;
	_rotMatrix.at(0,0) = u.x ; _rotMatrix.at(0,1) = u.y ; _rotMatrix.at(0,2) = u.z ;

	return true;
}

bool Turtle::randomTurnPitchRoll()
{
	return randomTurnPitchRoll((unsigned int)_angle);
}

bool Turtle::randomTurnPitchRoll(unsigned int angle)
{
	int r;
	for(int i=0; i<3; i++)
	{
		r = rand()%angle;
	    if(rand()%2) r *= -1;
	    universalRotation(r,i);
	}
	return true;
}


//================ INFLUENCNG OF STATE VARIABLES =======================

bool Turtle::multiplyLength(double multiplier)
{
	_length *= multiplier;
	return true;
}

bool Turtle::multiplyAngle(double multiplier)
{
	_angle *= multiplier;
	return true;
}



//================ STANDARD TURTLE TRANSLATIONS ========================

// --------- FORWARD MOVES ------------

bool Turtle::halfForwardDrawMove()
{
	return forwardDrawMove(_length/2.0);
}

bool Turtle::fullForwardDrawMove()
{
	return forwardDrawMove(_length);
}


vector<vmath::Vector4d> Turtle::getPosition()
{
	vector<vmath::Vector4d> vertexAndNormals;
	vertexAndNormals.push_back(_matrix*vmath::Vector4d(.0,.0,.0,1.0));
	vertexAndNormals.push_back(_rotMatrix*vmath::Vector4d(.0,1.0,.0,1.0));
	return vertexAndNormals;
}
