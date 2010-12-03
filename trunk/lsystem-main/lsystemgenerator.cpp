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

    m_MainLSystem = AbstractGenerator::createLSystem( file );

    //D0LSystem d = *dynamic_cast<D0LSystem *>(m_MainLSystem);
}

void LSystemGenerator::nextIteration()
{
    m_MainLSystem->nextIteration();
}

ParseableString * LSystemGenerator::getWord()
{
    if(m_Word)
        delete m_Word;

    LongString * word = NULL;

    word = m_MainLSystem->translate( );

//    D0LSystem l = *dynamic_cast<D0LSystem *>(m_MainLSystem.get());
//    int i = l.getIteration();
//    Log::get()->write(i);
	
	if(!word)
		return NULL;

//	BE CAREFUL - too long word can cause an exception
//	vrecko::logger.debugLog("Word: %s", word->toString().c_str() );
    Log::get()->write( word->toString() );

    return m_Word = new ParseableString( word );
}
