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
    std::map<string, string> defines;

    void processType(string);

public:
    LSFile();
    virtual void open(string &);
};
}

#endif // LSFILE_H
