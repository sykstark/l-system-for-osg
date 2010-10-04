#include "precompiled.h"
#include "lsobject.h"
#include "lparser.h"
#include "Configuration.h"
//#include "AbstractGrammar.h"
#include "LSFileGrammar.h"

XERCES_CPP_NAMESPACE_USE

namespace AP_LSystem {

LSObject::LSObject() : Ability("LSObject") 
{
//	pOSGGeode = NULL;
	pEOOwner = NULL;
	filename.clear();
	lod = 14;
	thickness = 1.0;
	angle = 15.0;
	length = 3.0;
	turtleType="";
}


LSObject::~LSObject() 
{
//	pOSGGeode = NULL;
	pEOOwner = NULL;
	filename.clear();
}

void LSObject::preInitialize() 
{
	pEOOwner = (EnvironmentObject *)pOwner;
}

void LSObject::postInitialize() 
{
	// Schedule the parent EO for regular updates
	getWorldPtr()->getSchedulerPtr()->addEntity(this, 60);
	
	AP_LSystem::Configuration::get()->loadCfgFile("vreckoAP_Garden.cfg");
	draw();
}

bool LSObject::loadXMLParameters(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *pParametersNode) 
{
	if (findXMLNode(pParametersNode, "Filename")) {
		ReaderWriter::getStringValue(this->filename, pParametersNode, "Filename");
	}
	if (findXMLNode(pParametersNode, "Word")) {
		ReaderWriter::getStringValue(this->word, pParametersNode, "Word");
	}
	if (findXMLNode(pParametersNode, "TurtleType")) {
		ReaderWriter::getStringValue(this->turtleType, pParametersNode, "TurtleType");
	}
	if (findXMLNode(pParametersNode, "LOD")) {
		this->lod = ReaderWriter::getIntValue(pParametersNode, "LOD");
	}
	if (findXMLNode(pParametersNode, "DefaultAngle")) {
		this->angle = ReaderWriter::getDoubleValue(pParametersNode, "DefaultAngle");
	}
	if (findXMLNode(pParametersNode, "DefaultThickness")) {
		this->thickness = ReaderWriter::getDoubleValue(pParametersNode, "DefaultThickness");
	}
	if (findXMLNode(pParametersNode, "DefaultLength")) {
		this->length = ReaderWriter::getDoubleValue(pParametersNode, "DefaultLength");
	}
	return true;
}

void LSObject::draw()
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray( v.get() );
	v->push_back( osg::Vec3( -1.f, 0.f, -1.f ) );
	v->push_back( osg::Vec3( 1.f, 0.f, -1.f ) );
	v->push_back( osg::Vec3( 1.f, 0.f, 1.f ) );
	v->push_back( osg::Vec3( -1.f, 0.f, 1.f ) );
	// Create an array of four colors.
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
	geom->setColorArray( c.get() );
	geom->setColorBinding( osg::Geometry::BIND_OVERALL );
	c->push_back( osg::Vec4( 1.f, 0.f, 0.f, 1.f ) );
	c->push_back( osg::Vec4( 0.f, 1.f, 0.f, 1.f ) );
	c->push_back( osg::Vec4( 0.f, 0.f, 1.f, 1.f ) );
	c->push_back( osg::Vec4( 1.f, 1.f, 1.f, 1.f ) );
	// Create an array for the single normal.
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	geom->setNormalArray( n.get() );
	geom->setNormalBinding( osg::Geometry::BIND_OVERALL );
	n->push_back( osg::Vec3( 0.f, -1.f, 0.f ) );
	// Draw a four-vertex quad from the stored data.
	geom->addPrimitiveSet(
	new osg::DrawArrays( osg::PrimitiveSet::QUADS, 0, 4 ) );
	// Add the Geometry (Drawable) to a Geode and
	// return the Geode.
	osg::Geode* geode = new osg::Geode();

	//geode->addDrawable(new ShapeDrawable(new Sphere(Vec3(), 1)));
	

	osg::ref_ptr<osg::Image> image = osgDB::readImageFile( "data\\ls\\bitmap.jpg" );

	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
	if(image.get())
	{
		tex->setImage( image.get() );

		osg::ref_ptr<osg::StateSet> state = geode->getOrCreateStateSet();
		state->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::OFF );
	}

	AddAnother( geom.get() );	
	
	geode->addDrawable( geom.get() );

	pEOOwner->addChild((osg::Group *)geode);
//	if (pOSGGeode != NULL)
//		pEOOwner->removeChild((osg::Group *)pOSGGeode);
//	pOSGGeode = geode;
}

void LSObject::AddAnother( osg::Geometry * geom)
{
	osg::ref_ptr<osg::Vec3Array> v = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
	cout << endl << "Elements number: " << v->getNumElements( ) << endl ;

	v->push_back( osg::Vec3( -1.f, 3.f, -1.f ) );
	v->push_back( osg::Vec3( 1.f, 3.f, -1.f ) );
	v->push_back( osg::Vec3( 1.f, 3.f, 1.f ) );
	v->push_back( osg::Vec3( -1.f, 3.f, 1.f ) );

	osg::ref_ptr<osg::Vec2Array> t = dynamic_cast<osg::Vec2Array*>(geom->getTexCoordArray(0));

	//cout << endl << "number of ref: " << t->referenceCount() << endl ;
	
	if(!t.get())
	{
		t = new osg::Vec2Array;
		cout << endl << "number of ref: " << t->referenceCount() << endl ;
		geom->setTexCoordArray(0,t.get());
	}

	cout << endl << "number of ref: " << t->referenceCount() << endl ;

	t->push_back( osg::Vec2( 0.f, 0.f ) );
	t->push_back( osg::Vec2( 1.f, 0.f ) );
	t->push_back( osg::Vec2( 1.f, 1.f ) );
	t->push_back( osg::Vec2( 0.f, 1.f ) );

	t->push_back( osg::Vec2( 0.f, 0.f ) );
	t->push_back( osg::Vec2( 1.f, 0.f ) );
	t->push_back( osg::Vec2( 1.f, 1.f ) );
	t->push_back( osg::Vec2( 0.f, 1.f ) );

	geom->addPrimitiveSet(
		new osg::DrawArrays( osg::PrimitiveSet::QUADS, 4, 4 ) );

	osg::PrimitiveSet * p = geom->getPrimitiveSet(geom->getPrimitiveSetList().size() - 1 );
	cout << endl << "Indices number: " << p->getNumIndices() << endl ;
	cout << endl << "number of ref: " << t->referenceCount() << endl ;
	//cout << endl << "Elements number: " << v->getNumElements( ) << endl ;
}
/*
void LSObject::draw() 
{
	AbstractGrammar * grammar;

	if( filename.empty() )
	{
		return;
	}

	unsigned int pos = filename.rfind( '.' );
	if( pos == std::string::npos )
	{
		return;
	}

	std::string ext = filename.substr( pos + 1, std::string::npos );

	if( ext == "ls" )
	{
		grammar = new LSFileGrammar( &filename );
		grammar->nextIteration();
	}

	
	unsigned int ttype = (turtleType == "jointed")?0:1;
	Lparser lp(thickness,angle,length,(unsigned int)lod, ttype);
	if(filename.empty())
	{
		lp.parse(this->word);
	}
	else
	{
		lp.loadFromFile(this->filename);
	}

	vector<vmath::Vector3d> vertexArray = lp.getVertex();
	vector<vmath::Vector3d> normalsArray = lp.getNormals();
	vector<Indices> indicesArray = lp.getIndices();
	

	// Create an object to store geometry in.
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	// Create an array of four vertices.
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array;
	geom->setVertexArray( v.get() );
	for(unsigned int i=0; i<vertexArray.size(); i++)
	{
		v->push_back( osg::Vec3( vertexArray[i].x, vertexArray[i].z, vertexArray[i].y));
	}

	// Create an array of four colors.
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array;
	geom->setColorArray( c.get() );
	geom->setColorBinding( osg::Geometry::BIND_PER_VERTEX );
	for(unsigned int i=0; i<indicesArray.size();i++)
	{
		for(unsigned int j=0; j<indicesArray.at(i).count; j++) 
			c->push_back( osg::Vec4(indicesArray.at(i).color.r, indicesArray.at(i).color.g, indicesArray.at(i).color.b, indicesArray.at(i).color.a ));
	}
	
	// Create an array for the single normal.
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array;
	geom->setNormalArray( n.get() );
	geom->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
	for(unsigned int i=0; i<normalsArray.size(); i++)
	{
		n->push_back( osg::Vec3( normalsArray[i].x, normalsArray[i].z, normalsArray[i].y));
	}
	// Draw a four-vertex quad from the stored data.
	unsigned int pos = 0;
	for(unsigned int i=0; i<indicesArray.size();i++)
	{
		geom->addPrimitiveSet(
			new osg::DrawArrays( indicesArray[i].primitiveSet, pos , indicesArray[i].count ) );
		pos += indicesArray[i].count;
	}
	

	// Add the Geometry (Drawable) to a Geode and
	// return the Geode.
	osg::Geode* geode = new osg::Geode();

	//geode->addDrawable( geom.get() );
	pEOOwner->addChild( (osg::Group *)geode);
	if (pOSGGeode != NULL)
		pEOOwner->removeChild( (osg::Group *)pOSGGeode);
	pOSGGeode = geode;
}*/
}