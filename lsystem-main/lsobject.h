#ifndef _LSOBJECT_H
#define _LSOBJECT_H

#include "abstractgenerator.h"

using namespace vrecko;

namespace AP_LSystem {
class LSObject: public Ability {
	private:
		// pointer to parent enviroment object
		EnvironmentObject* pEOOwner;

		// poiter to word generator from grammars
		AbstractGenerator * generator;

		// pointer to vertex array
		//osg::Vec3Array* pOSGVertexArray;

		// source filenames
		string grammarFile, configFile;

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

		void AddAnother( osg::Geometry * geom);

		void draw();

};
}
#endif
