#ifndef XMLFILE_H_
#define XMLFILE_H_

#include "abstractfile.h"

using std::string;

namespace AP_LSystem {
class XmlFile : public AbstractFile
{
private:
	xercesc::XercesDOMParser * m_FileParser;
    std::map<string, string> defines;

	void processParameters(xercesc::DOMNode *);
	void processRules(xercesc::DOMNode *);
	void processHomomorphisms(xercesc::DOMNode *);
	void processSubsystems(xercesc::DOMNode *);
	void processConstants(xercesc::DOMNode *);
public:
    XmlFile();

    virtual void open( std::string & );
};
}

#endif // XMLFILE_H_
