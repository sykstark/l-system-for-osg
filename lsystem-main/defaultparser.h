#ifndef DEFAULTPARSER_H_
#define DEFAULTPARSER_H_

#include "abstractparser.h"
#include "lsgeode.h"

namespace AP_LSystem {
class DefaultParser : public AbstractParser
{
private:
	
public:
	DefaultParser( osg::Group * owner );
	//DefaultParser( );
	~DefaultParser(void);

	virtual int parse( ParseableString * );
};
}

#endif