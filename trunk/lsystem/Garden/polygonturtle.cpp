#include "precompiled.h"
#include "PolygonTurtle.h"

PolygonTurtle::PolygonTurtle(const PolygonTurtle &copy)
{
	_matrix = copy.getMatrix();
	_rotMatrix = copy.getRotationMatrix();
	_length = copy.getLength();
	_angle = copy.getAngle();
	_def_length = copy.getDefLength();
	_def_angle = copy.getDefAngle();
	vector<vmath::Vector4d> vector4dArray;
	vector<vmath::Vector3d> vector3dArray;
	vector<Indices> indicesArray;

	vector4dArray = copy.getColors();
	this->colors.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector3dArray = copy.getVertices();
	this->verticesOutput.assign(vector3dArray.begin(),vector3dArray.end());

	vector3dArray.clear();

	vector3dArray = copy.getNormals();
	this->normalsOutput.assign(vector3dArray.begin(),vector3dArray.end());

	vector3dArray.clear();

	indicesArray = copy.getIndices();
	this->indicesOutput.assign(indicesArray.begin(),indicesArray.end());

	indicesArray.clear();
}

PolygonTurtle PolygonTurtle::operator=(const PolygonTurtle &copy)
{
	_matrix = copy.getMatrix();
	_rotMatrix = copy.getRotationMatrix();
	_length = copy.getLength();
	_angle = copy.getAngle();
	_def_length = copy.getDefLength();
	_def_angle = copy.getDefAngle();
	vector<vmath::Vector4d> vector4dArray;
	vector<vmath::Vector3d> vector3dArray;
	vector<Indices> indicesArray;

	vector4dArray = copy.getColors();
	this->colors.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector3dArray = copy.getVertices();
	this->verticesOutput.assign(vector3dArray.begin(),vector3dArray.end());

	vector3dArray.clear();

	vector3dArray = copy.getNormals();
	this->normalsOutput.assign(vector3dArray.begin(),vector3dArray.end());

	vector3dArray.clear();

	indicesArray = copy.getIndices();
	this->indicesOutput.assign(indicesArray.begin(),indicesArray.end());

	indicesArray.clear();

	return copy;
}



bool PolygonTurtle::universalRotation(double angle, int axis)
{
	vmath::Matrix4d rotation;
	switch(axis)
	{
	case 0:
		rotation = vmath::Matrix4d::createRotationAroundAxis(angle*TO_RAD,0.0,0.0);
		break;
	case 1:
		rotation = vmath::Matrix4d::createRotationAroundAxis(0.0,angle*TO_RAD,0.0);
		break;
	case 2:
		rotation = vmath::Matrix4d::createRotationAroundAxis(0.0,0.0,angle*TO_RAD);
		break;
	default:
		break;
	}
	_matrix = _matrix * rotation;
	_rotMatrix = _rotMatrix * rotation;

	return true;
}

bool PolygonTurtle::resetValues()
{
	_angle = _def_angle;
	_length = _def_length;

	return true;
}

bool PolygonTurtle::forwardDrawMove(double length)
{
	vmath::Matrix4d translation = vmath::Matrix4d::createTranslation(0.0,0.0,length);
	_matrix = _matrix * translation;
	vmath::Vector4d v(0.0,0.0,0.0,1.0);
	vmath::Vector4d n(0.0,1.0,0.0,1.0);
	v = _matrix*v;
	n = _rotMatrix*n;
	verticesOutput.push_back(vmath::Vector3d(v.x,v.y,v.z));
	normalsOutput.push_back(vmath::Vector3d(n.x,n.y,n.z));
	return true;
}

bool PolygonTurtle::initialize()
{
	vmath::Vector4d v(0.0,0.0,0.0,1.0);
	vmath::Vector4d n(0.0,1.0,0.0,1.0);
	v = _matrix*v;
	n = _rotMatrix*n;
	verticesOutput.push_back(vmath::Vector3d(v.x,v.y,v.z));
	normalsOutput.push_back(vmath::Vector3d(n.x,n.y,n.z));
	return true;
}

bool PolygonTurtle::finalize()
{
	indicesOutput.push_back(Indices(TRIANGLE_FAN, verticesOutput.size(), colors.at(_color)));
	return true;
}

inline void PolygonTurtle::increaseColorIndex()
{
	if(_color > colors.size()-2) _color = 0;
	else _color++;
}