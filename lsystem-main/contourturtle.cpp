#include "precompiled.h"
#include "contourturtle.h"


ContourTurtle::ContourTurtle(const ContourTurtle &copy)
{
	_thickness = copy.getThickness();
	_matrix = copy.getMatrix();
	_rotMatrix = copy.getRotationMatrix();
	_length = copy.getLength();
	_angle = copy.getAngle();
	_def_length = copy.getDefLength();
	_def_angle = copy.getDefAngle();
	_def_thickness = copy.getDefThickness();
	vector<vmath::Vector4d> vector4dArray;
	vector<vmath::Vector3d> vector3dArray;
	vector<Indices> indicesArray;

	vector4dArray = copy.getShapeVertices();
	this->shapeVertices.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getShapeNormals();
	this->shapeNormals.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getBottomCapVertices();
	this->bottomCapVertices.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getBottomCapNormals();
	this->bottomCapNormals.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getTopCapVertices();
	this->topCapVertices.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getTopCapNormals();
	this->topCapNormals.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

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

ContourTurtle ContourTurtle::operator=(const ContourTurtle &copy)
{
	_thickness = copy.getThickness();
	_matrix = copy.getMatrix();
	_rotMatrix = copy.getRotationMatrix();
	_length = copy.getLength();
	_angle = copy.getAngle();
	_def_length = copy.getDefLength();
	_def_angle = copy.getDefAngle();
	_def_thickness = copy.getDefThickness();
	vector<vmath::Vector4d> vector4dArray;
	vector<vmath::Vector3d> vector3dArray;
	vector<Indices> indicesArray;

	vector4dArray = copy.getShapeVertices();
	this->shapeVertices.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getShapeNormals();
	this->shapeNormals.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getBottomCapVertices();
	this->bottomCapVertices.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getBottomCapNormals();
	this->bottomCapNormals.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getTopCapVertices();
	this->topCapVertices.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

	vector4dArray = copy.getTopCapNormals();
	this->topCapNormals.assign(vector4dArray.begin(),vector4dArray.end());

	vector4dArray.clear();

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

bool ContourTurtle::forwardDrawMove(double length)
{
	vector<vmath::Vector4d>::iterator i;
	if(!bottomCapVertices.empty())
	{
		// vykresli FORWARD
		if(!forwardTriangulation()) return false;
		// ukonci predchozi FORWARD a pripravi se na editaci soucasneho
		bottomCapVertices = topCapVertices;
		bottomCapNormals = topCapNormals;
		topCapVertices.clear();
		topCapNormals.clear();
	}
	else
	{
		// jde o prvni FORWARD - vytvoreni spodni zakladny
		for(i= shapeVertices.begin(); i!= shapeVertices.end(); i++) bottomCapVertices.push_back(_matrix*(*i));
		for(i= shapeNormals.begin(); i!= shapeNormals.end(); i++) bottomCapNormals.push_back(_rotMatrix*(*i));
	}
	drawColor = actualColor;
	// vytvoreni matice translace
	vmath::Matrix4d translation = vmath::Matrix4d::createTranslation(0.0,0.0,length);
	_matrix = _matrix * translation;
	// vytvoreni horni zakladny - muze byt rotovana, cely dil neni ihned vykreslen
	for(i= shapeVertices.begin(); i!= shapeVertices.end(); i++) topCapVertices.push_back(_matrix*(*i));
	for(i= shapeNormals.begin(); i!= shapeNormals.end(); i++) topCapNormals.push_back(_rotMatrix*(*i));		
	return true;
}

bool ContourTurtle::universalRotation(double angle, int axis)
{
	//bude se upravovat horni capka...tudiz vymazeme vrch
	topCapVertices.clear();
	topCapNormals.clear();
	vmath::Matrix4d rotation;
	// vyber uhlu rotace a vytvoreni rotacni matice
	switch(axis)
	{
	case 0:
		rotation = vmath::Matrix4d::createRotationAroundAxis(angle*TO_RAD/2.0,0.0,0.0);
		break;
	case 1:
		rotation = vmath::Matrix4d::createRotationAroundAxis(0.0,angle*TO_RAD/2.0,0.0);
		break;
	case 2:
		rotation = vmath::Matrix4d::createRotationAroundAxis(0.0,0.0,angle*TO_RAD/2.0);
		break;
	default:
		break;
	}

	
	_matrix = _matrix * rotation;
	_rotMatrix = _rotMatrix * rotation;

	vector<vmath::Vector4d>::iterator i;
	if(!bottomCapVertices.empty())
	{
		for(i= shapeVertices.begin(); i!= shapeVertices.end(); i++) topCapVertices.push_back(_matrix*(*i));
		for(i= shapeNormals.begin(); i!= shapeNormals.end(); i++) topCapNormals.push_back(_rotMatrix*(*i));
	}
	_matrix = _matrix * rotation;
	_rotMatrix = _rotMatrix * rotation;
	return true;
}

bool ContourTurtle::multiplyThickness(double multiplier)
{
	_thickness *= multiplier;
	unsigned int detail = shapeVertices.size();
	shapeVertices.clear();
	for(unsigned int i=0; i<detail; i++)
	{
		shapeVertices.push_back(vmath::Vector4d((_thickness/2.0)*cos((double)i*2.0*_PI/(double)detail),
						                     (_thickness/2.0)*sin((double)i*2.0*_PI/(double)detail),
											  0.0,1.0));
	}
	topCapVertices.clear();
	topCapNormals.clear();
	vector<vmath::Vector4d>::iterator i;
	if(!bottomCapVertices.empty())
	{
		for(i= shapeVertices.begin(); i!= shapeVertices.end(); i++) topCapVertices.push_back(_matrix*(*i));
		for(i= shapeNormals.begin(); i!= shapeNormals.end(); i++) topCapNormals.push_back(_rotMatrix*(*i));
	}
	return true;
}

bool ContourTurtle::resetValues()
{
	_angle = _def_angle;
	_thickness = _def_thickness;
	_length = _def_length;

	unsigned int detail = shapeVertices.size();
	shapeVertices.clear();
	for(unsigned int i=0; i<detail; i++)
	{
		shapeVertices.push_back(vmath::Vector4d((_thickness/2.0)*cos((double)i*2.0*_PI/(double)detail),
						                     (_thickness/2.0)*sin((double)i*2.0*_PI/(double)detail),
											  0.0,1.0));
	}
	topCapVertices.clear();
	topCapNormals.clear();
	vector<vmath::Vector4d>::iterator i;
	if(!bottomCapVertices.empty())
	{
		for(i= shapeVertices.begin(); i!= shapeVertices.end(); i++) topCapVertices.push_back(_matrix*(*i));
		for(i= shapeNormals.begin(); i!= shapeNormals.end(); i++) topCapNormals.push_back(_rotMatrix*(*i));
	}

	return true;
}

//====================== OTHER ABILITIES ================

bool ContourTurtle::initialize()
{
	bottomCapVertices.clear();
	bottomCapNormals.clear();
	actualColor = _color;
	return true;
}

bool ContourTurtle::finalize()
{
	if(!bottomCapVertices.empty()) endTriangulation();
	return true;
}

bool ContourTurtle::forwardTriangulation()
{
	//PROZATIMNI ZAPNUTI AUTOROLLU
	autoRoll = false;


	if(topCapVertices.size() != bottomCapVertices.size())
	{
		cerr << "Cap size mismatch !";
		return false; 
	}
	
	vector<vmath::Vector4d>::iterator tvIt = topCapVertices.begin();
	vector<vmath::Vector4d>::iterator bvIt = bottomCapVertices.begin();
	vector<vmath::Vector4d>::iterator tnIt = topCapNormals.begin();
	vector<vmath::Vector4d>::iterator bnIt = bottomCapNormals.begin();
	if(autoRoll)
	{
		unsigned int topBegin, bottomBegin;
		double min;
		vmath::Vector4d dist;
		//vyhledani nejblizsich bodu
		dist = bottomCapVertices.at(0) - topCapVertices.at(0);
		min = dist.lengthSq();
		for(unsigned int i=0; i< bottomCapVertices.size();i++)
		{
			for(unsigned int j=0; j< topCapVertices.size();j++)
			{
				dist = bottomCapVertices.at(i) - topCapVertices.at(j);
				if(dist.lengthSq() < min)
				{
					min = dist.lengthSq();
					bottomBegin = j;
					topBegin = i;
				}
			}
		}
		// nastaveni iteratoru
		for(unsigned int i=0; i<bottomBegin;i++) { bvIt++;bnIt++; }
		for(unsigned int i=0; i<topBegin;i++) { tvIt++;tnIt++; }

		
		for(unsigned int i=0 ; i <= bottomCapVertices.size(); i++)
		{
			verticesOutput.push_back(vmath::Vector3d(bvIt->x,bvIt->y,bvIt->z));
			verticesOutput.push_back(vmath::Vector3d(tvIt->x,tvIt->y,tvIt->z));
			normalsOutput.push_back(vmath::Vector3d(bnIt->x,bnIt->y,bnIt->z));
			normalsOutput.push_back(vmath::Vector3d(tnIt->x,tnIt->y,tnIt->z));
			bvIt++;tvIt++;tnIt++;bnIt++;
			if(bvIt == bottomCapVertices.end())
			{
				bvIt = bottomCapVertices.begin();
				bnIt = bottomCapNormals.begin();
			}
			if(tvIt == topCapVertices.end())
			{
				tvIt = topCapVertices.begin();
				tnIt = topCapNormals.begin();
			}
		}
	}
	else
	{
		

		for(bvIt; bvIt != bottomCapVertices.end(); bvIt++,tvIt++,tnIt++,bnIt++)
		{
			verticesOutput.push_back(vmath::Vector3d(bvIt->x,bvIt->y,bvIt->z));
			verticesOutput.push_back(vmath::Vector3d(tvIt->x,tvIt->y,tvIt->z));
			normalsOutput.push_back(vmath::Vector3d(bnIt->x,bnIt->y,bnIt->z));
			normalsOutput.push_back(vmath::Vector3d(tnIt->x,tnIt->y,tnIt->z));
		}
		verticesOutput.push_back(vmath::Vector3d(bottomCapVertices.begin()->x,bottomCapVertices.begin()->y,bottomCapVertices.begin()->z));
		verticesOutput.push_back(vmath::Vector3d(topCapVertices.begin()->x,topCapVertices.begin()->y,topCapVertices.begin()->z));
		normalsOutput.push_back(vmath::Vector3d(bottomCapNormals.begin()->x,bottomCapNormals.begin()->y,bottomCapNormals.begin()->z));
		normalsOutput.push_back(vmath::Vector3d(topCapNormals.begin()->x,topCapNormals.begin()->y,topCapNormals.begin()->z));
	}
	indicesOutput.push_back(Indices(TRIANGLE_STRIP,shapeVertices.size()*2+2,colors.at(drawColor)));
	
	return true;
}

bool ContourTurtle::endTriangulation()
{
	if(topCapVertices.size() != bottomCapVertices.size()) return false; 
	vector<vmath::Vector4d>::iterator bvIt = bottomCapVertices.begin();
	vector<vmath::Vector4d>::iterator bnIt = bottomCapNormals.begin();
	
	vmath::Vector4d v = _matrix*vmath::Vector4d(.0,.0,.0,1.0);
	verticesOutput.push_back(vmath::Vector3d(v.x,v.y,v.z));
	normalsOutput.push_back(vmath::Vector3d(bnIt->x,bnIt->y,bnIt->z));
	for(bvIt; bvIt != bottomCapVertices.end(); bvIt++,bnIt++)
	{
		verticesOutput.push_back(vmath::Vector3d(bvIt->x,bvIt->y,bvIt->z));
		normalsOutput.push_back(vmath::Vector3d(bnIt->x,bnIt->y,bnIt->z));
	}
	verticesOutput.push_back(vmath::Vector3d(bottomCapVertices.begin()->x,bottomCapVertices.begin()->y,bottomCapVertices.begin()->z));
	normalsOutput.push_back(vmath::Vector3d(bottomCapNormals.begin()->x,bottomCapNormals.begin()->y,bottomCapNormals.begin()->z));
	indicesOutput.push_back(Indices(TRIANGLE_FAN,shapeVertices.size()+2,colors.at(drawColor)));
	return true;
}

inline void ContourTurtle::increaseColorIndex()
{
	if(_color > colors.size()-2) _color = 0;
	else _color++;
	actualColor = _color;
}


