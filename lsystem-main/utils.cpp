#include "precompiled.h"

#include "utils.h"
#include <fstream>
#include <sstream>
#include "lsystemexception.h"

using namespace AP_LSystem;

// odkomentuje radek od dvou zpetnych lomitek
void StringUtils::uncommentLine(std::string & line)
{
    unsigned int pos = line.find( "//" );
	if(pos != std::string::npos ) line.erase ( pos );
}

std::string StringUtils::processLine(std::fstream * pFile, std::stringstream & pLine)
{
    // clear stringstream 
    pLine.str("");
	// clear error control state
	pLine.clear();

	std::string s_line, word;

    if(!getline(*pFile, s_line))
	{
        throw ParsingException( "line expected" );
	}
	uncommentLine(s_line);
	pLine.str( s_line );	
    if(!(pLine >> word))
	{
        return "";
	}

	return word;
}	

