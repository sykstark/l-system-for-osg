#pragma once

#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H

#include <string>
#include <vector>

namespace AP_LSystem {
class AbstractFile
{

public:
    //AbstractFile(){};

    virtual void loadFromFile( std::string * ) = 0;
    virtual std::vector<std::string>* getHomomorphisms() = 0;
    virtual std::vector<std::string> * getRules() = 0;
    virtual std::string getAxiom() = 0;
};
}

#endif // ABSTRACTFILE_H
