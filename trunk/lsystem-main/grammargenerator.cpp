#include "precompiled.h"
#include "grammargenerator.h"
#include "lsfile.h"

using namespace AP_LSystem;

GrammarGenerator::GrammarGenerator(void)
{
}

GrammarGenerator::~GrammarGenerator(void)
{
}

void GrammarGenerator::loadFromFile(std::string & filename)
{
    AbstractFile * file;

    if( filename.empty() )
    {
        return;
    }

    unsigned int pos = filename.rfind( '.' );
    if( pos == std::string::npos )
    {
        return;
    }

    std::string ext = filename.substr( pos + 1, std::string::npos );

    if( ext == "ls" )
    {
        file = new LSFile;
    }
    else
    {
        throw FileException("unknown extension: ");
    }

    file->open(filename);

    createMainGrammar( );

    pMainGrammar->loadFromFile(file);
}

void GrammarGenerator::nextIteration()
{
    pMainGrammar->nextIteration();
}

void GrammarGenerator::createMainGrammar()
{

}
