#ifndef XMLFILE_H_
#define XMLFILE_H_

#include "abstractfile.h"

using std::string;

namespace AP_LSystem {
/**
  * This class opens and processes XML files with L-systems.
  */
class XmlFile : public AbstractFile
{
private:
    xercesc::XercesDOMParser * m_FileParser;    ///< XML parser
    std::map<string, string> defines;           ///< loaded constants with values - prepared for substitution

    /**
      * Process all nodes in Parameters entity
      * @param node <Parameters>
      */
    void processParameters(xercesc::DOMNode * node);

    /**
      * Process all nodes in Rules entity
      * @param node <Rules>
      */
    void processRules(xercesc::DOMNode * node);

    /**
      * Process all nodes in Homomorphisms entity
      * @param node <Homomorphisms>
      */
    void processHomomorphisms(xercesc::DOMNode * node);

    /**
      * Process all nodes in Subsystems entity
      * @param node <Subsystems>
      */
    void processSubsystems(xercesc::DOMNode * node);

    /**
      * Process all nodes in Constants entity
      * @param node <Constants>
      */
    void processConstants(xercesc::DOMNode * node);

    /**
      * Process all nodes in Types entity
      * @param node <Types>
      */
    void processType(xercesc::DOMNode * node);
public:
    XmlFile();

    /**
      * Opens XML file.
      * @param filename name of XML file
      */
    virtual void open( std::string & filename );
};
}

#endif // XMLFILE_H_
