#pragma once

#ifndef LSFILE_H
#define LSFILE_H

#include <vector>
#include <string>

#include "abstractfile.h"

namespace AP_LSystem{
class LSFile : public AbstractFile
{
private:
    std::vector<std::string> rules;
    std::vector<std::string> homomorhisms;
    std::string axiom;

public:
    LSFile();
    virtual void open(std::string *);
    virtual std::vector<std::string> * getHomomorphisms()
    {
        return &homomorhisms;
    };

    virtual std::vector<std::string> * getRules()
    {
        return &rules;
    };

    virtual std::string getAxiom()
    {
        return axiom;
    }
};
}

#endif // LSFILE_H
