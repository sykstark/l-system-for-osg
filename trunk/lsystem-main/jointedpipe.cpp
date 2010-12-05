#include "precompiled.h"
#include "jointedpipe.h"

using namespace AP_LSystem;

JointedPipe::JointedPipe(void)
{
}

JointedPipe::~JointedPipe(void)
{
}

int JointedPipe::preStep()
{
	return createLoftGeometryOpening( );
	//geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( osg::Vec3d( 0.0,0.0,0.0), 1.0 ) ) );
}

int JointedPipe::postStep()
{
	addContourLoftGeometry();

	addHemisphereGeometry( );

	return LS_OK;
}

int JointedPipe::addHemisphereGeometry()
{
	osg::Vec3dArray * v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
	osg::Vec3dArray * n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
	osg::Vec2dArray * t = dynamic_cast<osg::Vec2dArray*>(properties.geometry->getTexCoordArray(0));

	unsigned texSIndex = 0;
	// TEXTURE S COORDINATE
	//compute how much will be texture increment of one part of contour ( of one side of n-side contour polygon )
	double texSInc = properties.texRepeatingS / static_cast<double>(properties.contourDetail);

	
	double texTCoord, texSCoord = 0;
	double texTInc = 1.0 / properties.contourDetail;

	for(vector<osg::ref_ptr<osg::Vec3dArray> >::iterator i = properties.hemisphere.begin();i!=properties.hemisphere.end();i++,texSIndex++)
	{
		properties.geometry->addPrimitiveSet( 
			new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, v->size(), (*i)->size() ) );

		
		unsigned texTIndex = 0;
		texTCoord = properties.texCoordT + 1.0;
		for(osg::Vec3dArray::iterator j = (*i)->begin(); j!= (*i)->end();j++,texTIndex++)
		{
			if( properties.flags & TurtleProperties::MINIMIZE_TWIST )
			{
				v->push_back((*j * properties.radius) * properties.lastFrame * osg::Matrixd::translate(properties.matrix.getTrans()));
				n->push_back( properties.lastFrame.getRotate() * *j );
			}
			else
			{
				v->push_back((*j * properties.radius) * properties.matrix);
				n->push_back(properties.matrix.getRotate() * *j );
			}
			
			if( texTIndex%2 )
			{
				//liche
				texTCoord = properties.texCoordT + 1.0 - ( texTInc * ((texTIndex+1)/2) );
				texSCoord = (texSInc * texSIndex); 
			}
			else
			{
				//sude
				//texSCoord = (texSInc * (texSIndex + 1 < properties.hemisphere.size())?(texSIndex + 1):(0));
				texSCoord = (texSInc * (texSIndex + 1));
			}

			/*v->push_back(*cont * properties.radius) * properties.lastFrame * osg::Matrixd::translate(properties.matrix.getTrans());
			n->push_back( *j );*/
			t->push_back( osg::Vec2d(texSCoord,texTCoord));
		}
		//break;
		
	}

	return LS_OK;
}


/*
int JointedPipe::drawStep(double dist)
{
	

	osg::Matrixd translated = osg::Matrixd::translate(osg::Vec3d( 0.0, dist, 0.0 )) * properties.matrix;
	//drawDebugGeometry();

	osg::Vec3dArray * v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
	osg::Vec3dArray * n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
	unsigned int first = v->size();
	
	osg::Vec3dArray::iterator it;
	for(it = properties.contour->begin(); it != properties.contour->end(); it++ )
	{
		geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( *it, 0.3 ) ) );
		v->push_back( *it * properties.matrix );
		v->push_back( *it * translated );
		n->push_back( *it );
		n->push_back( *it );
	}

	it = properties.contour->begin();
	v->push_back( *it * properties.matrix );
	v->push_back( *it * translated );

	osg::Vec3d vn = *it;
	vn.normalize();
	n->push_back( vn );
	n->push_back( vn );

	properties.geometry->addPrimitiveSet( 
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, first, properties.contourDetail * 2 + 2 ) );

	for(vector<osg::ref_ptr<osg::Vec3dArray> >::iterator i = properties.hemisphere.begin();i!=properties.hemisphere.end();i++)
	{
		first = v->size();
		for(osg::Vec3dArray::iterator j = (*i)->begin(); j!= (*i)->end();j++)
		{
			v->push_back( *j * properties.radius * properties.matrix );
			n->push_back( *j );
		}
		properties.geometry->addPrimitiveSet( 
			new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, first, (*i)->size() ) );

	}

	

	osg::Quat q, q1 = osg::Quat( 0.0, UpVec ), q2 = osg::Quat( -osg::PI_2, UpVec );
	osg::Quat qHead( -osg::PI_2, UpVec );
	osg::Quat qLeft( 0.0, UpVec );
	osg::Quat qCont( 0.0, properties.contour->at(1)^HeadVec );
	osg::Quat qHead2( osg::PI_2, properties.contour->at(1)^HeadVec );

	q1.slerp( 0.5, qHead2, qCont );
	q.slerp( 0.5, qHead, qLeft );

	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( qHead * LeftVec * 5, 0.3 ) ) );
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( qCont * properties.contour->at(1) * 5, 0.3 ) ) );
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( q * LeftVec * 5, 0.3 ) ) );
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( q1 * properties.contour->at(1) * 5, 0.3 ) ) );
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( qLeft * LeftVec * 5, 0.3 ) ) );

	properties.matrix = translated;

	return LS_OK;
}*/

int JointedPipe::initialize()
{
	createCircleContour( );
	createHemisphere( );

	return LS_OK;
}

void JointedPipe::createHemisphere()
{
	if(!properties.hemisphere.empty())
		return;
	
	osg::ref_ptr<osg::Vec3dArray> strip;

	double k = 2.0f * PI / static_cast<double>(properties.contourDetail);
	double l = 0.5f * PI / static_cast<double>(properties.contourDetail);

	unsigned int i=0;
	for(unsigned int i=0; i < properties.contourDetail; i++) // cyklus kolem 
	{
		strip = new osg::Vec3dArray( );
		strip->push_back(HeadVec);
		//drawVector( HeadVec, properties.matrix, osg::Vec4d(0.0,1.0,0.0,1.0) );

		for(int j=(int)properties.contourDetail-1; j>=0; j--)
		{
			strip->push_back(osg::Vec3d(
				sin(i*k)*cos(j*l),
				sin(j*l),
				cos(i*k)*cos(j*l)));
			drawVector( *strip->rbegin(), properties.matrix, osg::Vec4d(1.0,0.0,0.0,1.0) );
			strip->push_back(osg::Vec3d(
				sin( (( (i+1) < properties.contourDetail)?(i+1):(0))*k)*cos(j*l),
				sin(j*l),
				cos( (( (i+1) < properties.contourDetail)?(i+1):(0))*k)*cos(j*l)));
			drawVector( *strip->rbegin(), properties.matrix, osg::Vec4d(0.0,1.0,0.0,1.0) );
		}
		properties.hemisphere.push_back(strip.get());
	}
}