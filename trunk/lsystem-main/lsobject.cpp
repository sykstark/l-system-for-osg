#include "precompiled.h"

#include <exception>
#include "lsobject.h"
#include "lparser.h"
#include "configuration.h"
//#include "AbstractGrammar.h"
//#include "LSFileGrammar.h"
#include "defaultparser.h"
#include "grammargenerator.h"
#include "log.h"


XERCES_CPP_NAMESPACE_USE

namespace AP_LSystem {

LSObject::LSObject() : Ability("LSObject") 
{
	pEOOwner = NULL;
}

LSObject::~LSObject() 
{
	pEOOwner = NULL;
}

void LSObject::preInitialize() 
{
	pEOOwner = (EnvironmentObject *)pOwner;
}

void LSObject::postInitialize() 
{
	// Schedule the parent EO for regular updates
	getWorldPtr()->getSchedulerPtr()->addEntity(this, 60);
	
	Configuration::get()->loadCfgFile( configFile );

	generator = new GrammarGenerator( );
	ParseableString * pWord;
	try
	{
		generator->loadFile( grammarFile );
		for ( int i = 0; i < 11; i++ )
		{
			generator->nextIteration();
		}
		pWord = generator->getWord();

	}
	catch( std::exception & e)
	{
		Log::write( e.what() );
		return;
	}

	AbstractParser * parser = new DefaultParser( pEOOwner );
	parser->parse( pWord );

//	osg::Geode * g = dynamic_cast<osg::Geode *>(pEOOwner->getChild(0));
//	g->addDrawable( new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.0f,0.0f,2.5),1.0,5.0) ) );
//	draw();

	
}

bool LSObject::loadXMLParameters(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *pParametersNode) 
{
	if (findXMLNode(pParametersNode, "GrammarFile")) {
		ReaderWriter::getStringValue(this->grammarFile, pParametersNode, "GrammarFile");
	}
	if (findXMLNode(pParametersNode, "ConfigFile")) {
		ReaderWriter::getStringValue(this->configFile, pParametersNode, "ConfigFile");
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
	//osg::Geode * geode = new osg::Geode();
	LSGeode * geode = new LSGeode(0);
	pEOOwner->addChild((osg::Group *)geode);

	//geode->addDrawable(new ShapeDrawable(new Sphere(Vec3(), 1)));
	

	osg::ref_ptr<osg::Image> image = osgDB::readImageFile( "data\\ls\\bitmap.jpg" );

	osg::ref_ptr<osg::Texture2D> tex = new osg::Texture2D;
	if(image.get())
	{
		tex->setImage( image.get() );

		osg::ref_ptr<osg::StateSet> state = geode->getOrCreateStateSet();
		state->setTextureAttributeAndModes( 0, tex.get(), osg::StateAttribute::ON );
	}

	//AddAnother( geom.get() );	
	
	//geode->addDrawable( geom.get() );
	osg::Cylinder * cldr = new osg::Cylinder(osg::Vec3(1.0f,1.0f,2.0f),1.0,10.0);
	cldr->setRotation( osg::Quat( 15.0, osg::Vec3f( 0.5, 0.25, 0.66 ) ) );
	geode->addDrawable( new osg::ShapeDrawable( cldr) );

	
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