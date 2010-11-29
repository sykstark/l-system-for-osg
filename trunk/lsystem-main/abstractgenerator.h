#ifndef ABSTRACTGENERATOR_H_
#define ABSTRACTGENERATOR_H_

#include <string>
#include "parseablestring.h"
#include "abstractlsystem.h"

namespace AP_LSystem {

/**
  * L-system generator is a main class ford word generation. It controls one
  * L-system and use them for words generating. This is a base abstract class
  * which is an interface for communication with higher levels.
  */
class AbstractGenerator
{
protected:
    ParseableString * m_Word;
    AbstractLSystem * m_MainLSystem;
	
public:
	AbstractGenerator(void)
	{
        m_Word = NULL;
        m_MainLSystem = NULL;
	}

	~AbstractGenerator(void)
	{
		// TODO
	}

    /**
      * Loads an L-system. File is specified by it's filename.
      * @param filename filename of file in one of supported L-system formats
      */
    virtual void loadFile( std::string & filename ) = 0;

    /**
      * Process one iteration on generator's main L-system.
      */
    virtual void nextIteration() = 0;

    /**
      * Get generated word in format suitable for parsing and intepreting.
      * @return generated word
      */
    virtual ParseableString * getWord() = 0;
	
    /**
      * Not implemented
      */
    virtual void saveWordToFile( std::string & );

    /**
      * Not implemented
      */
    virtual void loadWordFromFile( std::string & );

    /**
      * Creates an L-system according to type. It chooses from all AbstractLSystem childs
      * and select the best suitable L-system implementation for this type of L-system.
      * @param file file with loaded L-system
      * @return created L-system
      */
    static AbstractLSystem * createLSystem( AbstractFile * file );
};
}

#endif
