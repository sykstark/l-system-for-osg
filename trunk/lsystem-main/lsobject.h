#ifndef _LSOBJECT_H
#define _LSOBJECT_H

#include "precompiled.h"

#include "abstractgenerator.h"
/*
#include <vreckoDB/ReaderWriter.h>

#include <vrecko/Ability.h>
#include <vrecko/Scene.h>
#include <vrecko/EnvironmentObject.h>
#include <vrecko/Logger.h>

#include <helpers/XercesXMLUtils.h>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

#include <osgDB/ReadFile>

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/CullFace>
#include <osg/LightModel>
#include <vrecko/Scene.h>

#include <sstream>
#include <vector>
#include <set>
#include <map>
*/
using namespace vrecko;
using namespace vreckoDB;

namespace AP_LSystem {

class LSObject: public Ability {
	private:
		// pointer to parent enviroment object
		EnvironmentObject* pEOOwner;

		// poiter to generator of words from grammars
		AbstractGenerator * generator;

		// pointer to a geode of the room
		//osg::Geode* pOSGGeode;

		// pointer to vertex array
		osg::Vec3Array* pOSGVertexArray;

		// source filename
		string filename;

		string word, turtleType;
		int lod;
		double length, angle, thickness;

	public:
		// constructor
		LSObject();

		// destructor
		virtual ~LSObject();

		// called as the first initialization method, before loadXMLParameters
		virtual void preInitialize();

		// called as the last initialization method, after loadXMLParameters
		virtual void postInitialize();

		// loads XML parameters from given node
		virtual bool loadXMLParameters(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *pParametersNode);

//		inline osg::Geode* getGeode() { return pOSGGeode; }

		void AddAnother( osg::Geometry * geom);

		void draw();

};

}

#endif
