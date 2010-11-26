#include "precompiled.h"

#include "utils.h"
#include <fstream>
#include <sstream>
#include "lsystemexception.h"
#include "boost/lexical_cast.hpp"

using namespace AP_LSystem;
using boost::lexical_cast;

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

int StringUtils::processVector( std::string str, std::vector<double> & v )
{
    unsigned i = 0;
	while( true )
    {
		i = str.find_first_of(",)(", i);
        if ( i == std::string::npos )
            break;
        str.replace(i,1," ");
    }

	std::stringstream ss;
	ss << str;

	while( ss >> str )
		v.push_back( lexical_cast<double>(str) );

	return 0;
}
