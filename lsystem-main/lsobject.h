#ifndef LSOBJECT_H_
#define LSOBJECT_H_

#include "abstractgenerator.h"
#include "abstractinterpret.h"

using namespace vrecko;

namespace AP_LSystem {
/** 
  * Main class of L-system object
  */
class LSObject: public Ability {
	private:
		EnvironmentObject* m_EOOwner;		///< pointer to parent enviroment object		
		AbstractGenerator * m_Generator;	///< pointer to word generator from L-system
		AbstractInterpret * m_Interpreter;	///< pointer to turtle graphics interpreter
		string m_LSystemFileName;			///< filename of L-system file
		string m_ConfigFileName;			///< filename of configuration file

	public:
		/**
		  * constructor
		  */
		LSObject();

		/**
		  * destructor
		  */
		virtual ~LSObject();

		/**
		  * called as the first initialization method, before loadXMLParameters
		  */
		virtual void preInitialize();

		/**
		  * called as the last initialization method, after loadXMLParameters
		  */
		virtual void postInitialize();

		/**
		  * loads XML parameters from given node
		  */
		virtual bool loadXMLParameters(XERCES_CPP_NAMESPACE_QUALIFIER DOMNode *pParametersNode);
};
}
#endif
