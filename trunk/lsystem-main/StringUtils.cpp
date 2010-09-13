/*#ifdef _MSC_VER
#include "precompiled.h"
#endif*/

#include "StringUtils.h"
#include <fstream>
#include "lsystemexception.h"

using namespace AP_LSystem;

// odkomentuje radek od dvou zpetnych lomitek
void StringUtils::uncommentLine(std::string & line)
{
    unsigned int pos = line.find( "//" );
	if(pos != std::string::npos ) line.erase ( pos );

/*	string temp;
	if((ss >> temp)&&((temp.size()<2)||(temp[0]!='/')&&(temp[1]!='/')))
	{
		return false;
	}
	else 
	{
		return true;
	}*/
}

std::string StringUtils::processLine(std::fstream * pFile, std::stringstream & pLine)
{
	pLine.clear( );

	std::string s_line, word;

    if(!getline(*pFile, s_line))
	{
        throw ParsingException( "line expected" );
	}
	uncommentLine(s_line);
	pLine << s_line;	
    if(!(pLine >> word))
	{
        throw LSystemException( "read error" );
	}

	//*pLine >> word;
	return word;
}	
