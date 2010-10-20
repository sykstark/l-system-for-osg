//#pragma once

#ifndef LSFILE_H
#define LSFILE_H

#include <vector>
#include <map>
#include <string>

#include "abstractfile.h"

using std::string;

namespace AP_LSystem{
class LSFile : public AbstractFile
{
private:
    std::vector<std::string> rules;
    std::vector<std::string> homomorhisms;
    std::vector<string> grammars;
    std::map<std::string, std::string> defines;
    std::string axiom;

    void processDefines();
    void processType(std::string);

public:
    LSFile();
    virtual void open(std::string &);
    virtual std::vector<std::string> * getHomomorphisms()
    {
        return &homomorhisms;
    }

    virtual std::vector<std::string> * getRules()
    {
        return &rules;
    }

    virtual std::string & getAxiom()
    {
        return axiom;
    }

    virtual std::vector<string> * getGrammarFileNames()
    {
        return &grammars;
    }
};
}

#endif // LSFILE_H
