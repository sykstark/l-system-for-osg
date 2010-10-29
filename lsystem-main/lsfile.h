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
    std::vector<string> rules;
    std::vector<string> homomorhisms;
    std::vector<string> grammars;
    std::map<string, string> defines;
    string axiom;

    void processType(string);

public:
    LSFile();
    virtual void open(string &);
	void substitute(std::map<string, string> &);

    virtual std::vector<string> * getHomomorphisms()
    {
        return &homomorhisms;
    }

    virtual std::vector<string> * getRules()
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
