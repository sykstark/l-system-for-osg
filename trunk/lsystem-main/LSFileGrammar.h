//#pragma once

#ifndef LSFILEGRAMMAR_H_
#define LSFILEGRAMMAR_H_

#include <map>

#include "configuration.h"
#include "abstractgrammar.h"
#include "rule.h"

namespace AP_LSystem{
class LSFileGrammar : public AbstractGrammar
{
private:
	static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC;


    bool getEndOfStatic(string *, string::iterator &, string::iterator &);

public:
    LSFileGrammar( AbstractFile * );
	~LSFileGrammar(void);













};
}

#endif
