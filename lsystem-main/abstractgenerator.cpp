#include "precompiled.h"

#include <fstream>
#include <sys/stat.h>
#include <errno.h>
#include "abstractgenerator.h"
#include "d0lsystem.h"
#include "parstoch0lsystem.h"
#include "par2lsystem.h"


using namespace AP_LSystem;

AbstractLSystem * AbstractGenerator::createLSystem( AbstractFile * file )
{
    AbstractLSystem * lsystem;
    if( D0LSystem::isCapable( file->type() ) )
        lsystem = new D0LSystem( file );
    else if( ParStoch0LSystem::isCapable( file->type() ) )
        lsystem = new ParStoch0LSystem( file );
    else if( Par2LSystem::isCapable( file->type() ) )
        lsystem = new Par2LSystem( file );
    else
        throw ParsingException("non of L-systems fulfils the conditions");

    return lsystem;
}

void AbstractGenerator::saveWordToFile( std::string & filename )
{
    if(!m_Word)
    {
        if( !m_MainLSystem )
        {
            throw LSystemException( "No L-system loaded" );
        }
        getWord();
        if( !m_Word )
        {
            throw LSystemException( "Word not translated" );
        }
    }
    struct stat fileStatus;
    int iretStat = stat(filename.c_str(), &fileStatus);
    if( iretStat == ENOTDIR )
        throw FileException("A component of the path is not a directory.");
    else if( iretStat == EACCES )
        throw FileException("Permission denied.");
    else if( iretStat == ENAMETOOLONG )
        throw FileException("File can not be read\n");

    std::ofstream * out = new std::ofstream(filename.c_str());
    if( !out->is_open() )
    {
        throw FileException( "File " + filename + " cannot be created" );
    }

    *out << (*m_Word);

//    Configuration::get()->writeAll();

    out->close();
}

void AbstractGenerator::loadWordFromFile( std::string & filename )
{
    struct stat fileStatus;
    int iretStat = stat(filename.c_str(), &fileStatus);
    if( iretStat == ENOTDIR )
        throw FileException("A component of the path is not a directory.");
    else if( iretStat == EACCES )
        throw FileException("Permission denied.");
    else if( iretStat == ENAMETOOLONG )
        throw FileException("File can not be read\n");

    std::fstream * out = new std::fstream(filename.c_str());
    if( !out->is_open() )
    {
        throw FileException( "File " + filename + " cannot be opened" );
    }
}
