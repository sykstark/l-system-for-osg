#include "precompiled.h"
#include "grammargenerator.h"
#include "LSFileGrammar.h"
#include "lsfile.h"

using namespace AP_LSystem;

GrammarGenerator::GrammarGenerator(void)
{
}

GrammarGenerator::~GrammarGenerator(void)
{
}

void GrammarGenerator::loadFile(std::string & filename)
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

	if( LSFileGrammar::isCapable( file->type() ) )
		pMainGrammar = new LSFileGrammar( file );
	else
		throw ParsingException("non of grammars fulfils the conditions");
}

void GrammarGenerator::nextIteration()
{
    pMainGrammar->nextIteration();
}

ParseableString * GrammarGenerator::getWord()
{
    if(pWord)
        delete pWord;

    char * word = NULL;
    unsigned int length;
    pMainGrammar->translate( word, length );

    return pWord = new ParseableString(word, length);
}
