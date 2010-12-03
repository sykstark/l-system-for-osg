#ifndef LSFILE_H_
#define LSFILE_H_

#include <vector>
#include <map>
#include <string>

#include "abstractfile.h"

using std::string;

namespace AP_LSystem{
/**
  * Class for loading file in *.ls format
  */
class LSFile : public AbstractFile
{
private:
    std::map<string, string> defines;

    /**
      * Process expression with type constants. Type of L-system is set according to parsed expression as
      * binary sum of parsed values.
      * @param expression expression string in following format (TYPE1|TYPE2|TYPE3)
      */
    void processType( std::string expression );

public:
    LSFile();
    /**
      * Open *.ls file and parse. All values are either set as COnfiguration properties or stored
      * to AbstractFile containers and variables.
      * @param filename name of file to open
      */
    virtual void open(string & filename);
};
}

#endif // LSFILE_H
