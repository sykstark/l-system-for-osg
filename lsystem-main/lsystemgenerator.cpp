#include "precompiled.h"
#include "lsystemgenerator.h"
#include "parstoch0lsystem.h"
#include "par2lsystem.h"
#include "d0lsystem.h"
#include "lsfile.h"
#ifdef _MSC_VER
#include "xmlfile.h"
#endif
#include "log.h"

using namespace AP_LSystem;

LSystemGenerator::LSystemGenerator(void)
{
}

LSystemGenerator::~LSystemGenerator(void)
{
}

void LSystemGenerator::loadFile(std::string & filename)
{
    AbstractFile * file;

    if( filename.empty() )
    {
        throw FileException("Filename of L-system file is empty");
    }

    unsigned int pos = filename.rfind( '.' );
    if( pos == std::string::npos )
    {
        throw FileException("Filename of L-system file has no extension");
    }

    std::string ext = filename.substr( pos + 1, std::string::npos );

    if( ext == "ls" )
    {
        file = new LSFile;
    }
#ifdef _MSC_VER
	else if( ext == "xml" )
    {
        file = new XmlFile;
    }
#endif
    else
    {
        throw FileException("Filename of L-system file has unknown extension: " + ext);
    }

    file->open(filename);

    pMainLSystem = AbstractGenerator::createLSystem( file );
}

void LSystemGenerator::nextIteration()
{
    pMainLSystem->nextIteration();
}

ParseableString * LSystemGenerator::getWord()
{
    if(pWord)
        delete pWord;

    LongString * word = NULL;

    word = pMainLSystem->translate( );
	
	if(!word)
		return NULL;

//	BE CAREFUL - too long word can cause an exception
	vrecko::logger.debugLog("Word: %s", word->toString().c_str() );

    return pWord = new ParseableString( word );
}
