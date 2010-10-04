#ifndef DEFAULTPARSER_H_
#define DEFAULTPARSER_H_

#include "abstractparser.h"
#include "lsgeode.h"

namespace AP_LSystem {
class DefaultParser : public AbstractParser
{
private:
	
public:
	DefaultParser(void);
	//DefaultParser( );
	~DefaultParser(void);

	virtual void parse( LongString * );
};
}

#endif