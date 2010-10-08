//#pragma once

#ifndef LSFILE_H
#define LSFILE_H

#include <vector>
#include <map>
#include <string>

#include "abstractfile.h"

using std::string;

enum GrammarCapabilities
{
    LS_0L               = 0x00000001,
    LS_1L               = 0x00000002,
    LS_2L               = 0x00000004,
    LS_kL               = 0x00000080,
    LS_DETERMINISTIC    = 0x00000100,
    LS_STOCHASTIC       = 0x00000200,
    LS_PARAMETRIC       = 0x00010000,
};

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
