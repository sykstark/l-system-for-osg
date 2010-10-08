//#pragma once

#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H

#include <string>
#include <vector>
#include <map>

namespace AP_LSystem {
class AbstractFile
{
protected:
    unsigned int _type;
public:
    AbstractFile(): _type(0){}
    virtual void open( std::string * ) = 0;
    virtual std::vector<std::string>* getHomomorphisms() = 0;
    virtual std::vector<std::string> * getRules() = 0;
    virtual std::string & getAxiom() = 0;
    virtual std::vector<std::string> * getGrammarFileNames() = 0;
    unsigned int type()
    {
        return _type;
    }
};
}

#endif // ABSTRACTFILE_H
