#include "precompiled.h"
#include "jointedpipe.h"

using namespace AP_LSystem;

JointedPipe::JointedPipe(void)
{
	//createContour();
}

JointedPipe::~JointedPipe(void)
{
}

int JointedPipe::preStep()
{
	//geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( osg::Vec3d( 0.0,0.0,0.0), 1.0 ) ) );
	return 0;
}

int JointedPipe::postStep()
{
	return 0;
}

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

	/*properties.geometry->addPrimitiveSet( 
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, first, properties.contourDetail * 2 + 2 ) );*/

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

	

	/*osg::Quat q, q1 = osg::Quat( 0.0, UpVec ), q2 = osg::Quat( -osg::PI_2, UpVec );
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
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( qLeft * LeftVec * 5, 0.3 ) ) );*/

	properties.matrix = translated;

	return LS_OK;
}

int JointedPipe::initialize()
{
	createCircleContour( );
	createHemisphere( );

	return LS_OK;
}

int JointedPipe::finalize()
{
	return LS_OK;
}

void JointedPipe::createHemisphere()
{
	if(!properties.hemisphere.empty())
		return;
	
	// TODO ref_ptr
	osg::Vec3dArray * strip;

	double k = 2.0f * PI / (double)properties.contourDetail;
	double l = PI / (double)properties.contourDetail * 2.0f;

	unsigned int i=0;
	//for(unsigned int i=0; i < properties.contourDetail-1; i++) // cyklus kolem 
	{
		strip = new osg::Vec3dArray( );
		strip->push_back(HeadVec);

		//int j=(int)properties.contourDetail-2;
		for(int j=(int)properties.contourDetail-2; j>=0; j--)
		{
			strip->push_back(osg::Vec3d(
				sin((double)i*k)*cos((double)j*l),
				-sin((double)j*0),
				-cos((double)i*k)*cos((double)j*l)				 
							));
			strip->push_back(osg::Vec3d(
				sin((double)(i+1)*k)*cos((double)j*l),
				-sin((double)j*0) ,
				-cos((double)(i+1)*k)*cos((double)j*l)				
							));
		}
		properties.hemisphere.push_back(strip);
	}

	
/*	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( q1 * LeftVec * 5, 0.3 ) ) );
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( q2 * LeftVec * 5, 0.3 ) ) );
	geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( q * LeftVec * 5, 0.3 ) ) );*/
/*	for( unsigned int i = 0; i < properties.contourDetail; i++ )

	properties.hemisphere = new osg::Vec3dArray( );*/

}