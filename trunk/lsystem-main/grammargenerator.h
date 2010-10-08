#ifndef FROMFILEGENERATOR_H_
#define FROMFILEGENERATOR_H_

#include "abstractgenerator.h"

class FromFileGenerator :
	public AbstractGenerator
{
public:
	FromFileGenerator(void);
	~FromFileGenerator(void);

    virtual void loadFromFile( std::string & str );
    virtual void nextIteration();
};
#endif
