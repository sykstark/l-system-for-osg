#ifndef _LSOBJECT_H
#define _LSOBJECT_H

#include "abstractgenerator.h"

using namespace vrecko;

namespace AP_LSystem {
class LSObject: public Ability {
	private:
		// pointer to parent enviroment object
		EnvironmentObject* pEOOwner;

		osg::ref_ptr<osgShadow::ShadowedScene> LSScene;

		// poiter to word generator from L-system
		AbstractGenerator * generator;

		// source filenames
		string lsystemFile, configFile;

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
};
}
#endif
