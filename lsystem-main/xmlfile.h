#ifndef XMLFILE_H_
#define XMLFILE_H_

#include "abstractfile.h"

using std::string;

namespace AP_LSystem {
class XmlFile : public AbstractFile
{
private:
    std::map<string, string> defines;
public:
    XmlFile();

    virtual void open( std::string & );
};
}

#endif // XMLFILE_H_
