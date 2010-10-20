#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H

#include <string>
#include <vector>
#include <map>

namespace AP_LSystem {
/*enum GrammarCapabilities
{
    LS_0L               = 0x00000001,
    LS_1L_LEFT          = 0x00000002,
    LS_1L_RIGHT         = 0x00000004,
    LS_2L               = 0x00000008,
    LS_kL               = 0x00000080,
    LS_DETERMINISTIC    = 0x00000100,
    LS_STOCHASTIC       = 0x00000200,
    LS_PARAMETRIC       = 0x00010000,
};*/

class AbstractFile
{
protected:
    unsigned int _type;
    std::string _name;
public:
    AbstractFile(): _type(0){}
    virtual void open( std::string & ) = 0;
    virtual std::vector<std::string>* getHomomorphisms() = 0;
    virtual std::vector<std::string> * getRules() = 0;
    virtual std::string & getAxiom() = 0;
    virtual std::vector<std::string> * getGrammarFileNames() = 0;
    unsigned int type()
    {
        return _type;
    }

    std::string & name()
    {
        return _name;
    }
};
}

#endif // ABSTRACTFILE_H
