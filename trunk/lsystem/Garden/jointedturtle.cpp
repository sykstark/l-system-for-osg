#include "precompiled.h"
#include "jointedturtle.h"


JointedTurtle::JointedTurtle(const JointedTurtle &copy)
{
	_thickness = copy.getThickness();
	_matrix = copy.getMatrix();
	_rotMatrix = copy.getRotationMatrix();
	_length = copy.getLength();
	_angle = copy.getAngle();
	_def_length = copy.getDefLength();
	_def_angle = copy.getDefAngle();
	_def_thickness = copy.getDefThickness();
	vector<vector<vmath::Vector4d>> vectorArray;
	vector<vmath::Vector4d> vector4dArray;
	vector<vmath::Vector3d> vector3dArray;
	vector<Indices> indicesArray;

	vectorArray = copy.getHemisphere();
	for(vector<vector<vmath::Vector4d>>::iterator i = vectorArray.begin(); i!= vectorArray.end(); i++)
	{
		vector4dArray.assign(i->begin(),i->end());
		this->hemisphere.push_back(vector4dArray);
	}

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

JointedTurtle JointedTurtle::operator=(const JointedTurtle &copy)
{
	_thickness = copy.getThickness();
	_matrix = copy.getMatrix();
	_rotMatrix = copy.getRotationMatrix();
	_length = copy.getLength();
	_angle = copy.getAngle();
	_def_length = copy.getDefLength();
	_def_angle = copy.getDefAngle();
	_def_thickness = copy.getDefThickness();
	vector<vector<vmath::Vector4d>> vectorArray;
	vector<vmath::Vector4d> vector4dArray;
	vector<vmath::Vector3d> vector3dArray;
	vector<Indices> indicesArray;

	vectorArray = copy.getHemisphere();
	for(vector<vector<vmath::Vector4d>>::iterator i = vectorArray.begin(); i!= vectorArray.end(); i++)
	{
		vector4dArray.assign(i->begin(),i->end());
		this->hemisphere.push_back(vector4dArray);
	}

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

bool JointedTurtle::forwardDrawMove(double length)
{
	vector<vmath::Vector4d>::iterator i;
	if(!bottomCapVertices.empty())
	{
		// vykresli FORWARD
		if(!forwardTriangulation()) return false;
		if(!sphereTriangulation(false)) return false;
		// ukonci predchozi FORWARD a pripravi se na editaci soucasneho
		topCapVertices.clear();
		topCapNormals.clear();
		bottomCapVertices.clear();
		bottomCapNormals.clear();	
	}
	// vytvoreni spodni zakladny pro novy forward
	for(i= shapeVertices.begin(); i!= shapeVertices.end(); i++) bottomCapVertices.push_back(_matrix*(*i));
	for(i= shapeNormals.begin(); i!= shapeNormals.end(); i++) bottomCapNormals.push_back(_rotMatrix*(*i));
	// prirazeni aktualni barvy novemu forwardu
	drawColor = actualColor;
	// vytvoreni matice translace
	vmath::Matrix4d translation = vmath::Matrix4d::createTranslation(0.0,0.0,length);
	_matrix = _matrix * translation;
	// vytvoreni horni zakladny - muze byt rotovana, cely dil neni ihned vykreslen
	for(i= shapeVertices.begin(); i!= shapeVertices.end(); i++) topCapVertices.push_back(_matrix*(*i));
	for(i= shapeNormals.begin(); i!= shapeNormals.end(); i++) topCapNormals.push_back(_rotMatrix*(*i));

	return true;
}

bool JointedTurtle::universalRotation(double angle, int axis)
{
	//bude se upravovat horni capka...tudiz vymazeme vrch
	vmath::Matrix4d rotation;
	// vyber uhlu rotace a vytvoreni rotacni matice
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

bool JointedTurtle::multiplyThickness(double multiplier)
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

bool JointedTurtle::resetValues()
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

bool JointedTurtle::initialize()
{
	bottomCapVertices.clear();
	bottomCapNormals.clear();
	actualColor = _color;
	return true;
}

bool JointedTurtle::finalize()
{
	if(!bottomCapVertices.empty())
	{
		if(!forwardTriangulation()) return false;
		if(!sphereTriangulation(true)) return false;
	}
	return true;
}

bool JointedTurtle::forwardTriangulation()
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

bool JointedTurtle::sphereTriangulation(bool up)
{
	vmath::Matrix4d m1, m2;
	if(up)
	{
		m1=_matrix;
		m2=_rotMatrix;
	}
	else
	{
		vmath::Matrix4d r = vmath::Matrix4d::createRotationAroundAxis(_PI,0.0,0.0);
		m1 = _matrix*r;
		m2 = _rotMatrix*r;
	}
		
	vmath::Vector4d v;
	vmath::Vector3d n;
	for(vector<vector<vmath::Vector4d>>::iterator i = hemisphere.begin();i!=hemisphere.end();i++)
	{
		for(vector<vmath::Vector4d>::iterator j = i->begin(); j!= i->end();j++)
		{
			v = (*j)*(_thickness/2.0);
			v.w = 1.0;
			v = m1*v;
			verticesOutput.push_back(vmath::Vector3d(v.x,v.y,v.z));
			v = m2*(*j);
			n=vmath::Vector3d(v.x,v.y,v.z);
			n.normalize();
			normalsOutput.push_back(n);
		//	if(abs(v.lengthSq() - 1.0) > 0.0001) cout << "normala: " << v.length() << endl;
		}
		indicesOutput.push_back(Indices(TRIANGLE_STRIP,i->size(),colors.at(drawColor)));
	}	
	return true;
}

inline void JointedTurtle::increaseColorIndex()
{
	if(_color > colors.size()-2) _color = 0;
	else _color++;
	actualColor = _color;
}


void JointedTurtle::createHemisphere(unsigned int detail)
{
	if(detail<3) detail = 3;
	vmath::Vector4d v(0.0,0.0,1.0,1.0);
	vector<vmath::Vector4d> strip;
	for(unsigned int i=0; i < detail-1; i++) // cyklus kolem 
	{
		strip.push_back(v);
		for(int j=(int)detail-2; j>=0; j--)
		{
			strip.push_back(vmath::Vector4d(
				cos((double)i*2.0*_PI/(double)detail)*cos((double)j*_PI/((double)detail*2.0)),
				sin((double)i*2.0*_PI/(double)detail)*cos((double)j*_PI/((double)detail*2.0)),
				sin((double)j*_PI/((double)detail*2.0)),1.0));
			strip.push_back(vmath::Vector4d(
				cos((double)(i+1)*2.0*_PI/(double)detail)*cos((double)j*_PI/((double)detail*2.0)),
				sin((double)(i+1)*2.0*_PI/(double)detail)*cos((double)j*_PI/((double)detail*2.0)),
				sin((double)j*_PI/((double)detail*2.0)),1.0));
		}
		hemisphere.push_back(strip);
		strip.clear();
	}
	strip.push_back(v);
	for(int j=(int)detail-2; j>=0; j--)
	{
		strip.push_back(vmath::Vector4d(
			cos((double)(detail-1)*2.0*_PI/(double)detail)*cos((double)j*_PI/((double)detail*2.0)),
			sin((double)(detail-1)*2.0*_PI/(double)detail)*cos((double)j*_PI/((double)detail*2.0)),
			sin((double)j*_PI/((double)detail*2.0)),1.0));
		strip.push_back(vmath::Vector4d(
			cos((double)j*_PI/((double)detail*2.0)),
			0.0,
			sin((double)j*_PI/((double)detail*2.0)),1.0));
	}
	hemisphere.push_back(strip);
	strip.clear();
}




