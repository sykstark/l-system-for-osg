/*#ifdef _MSC_VER
#include "precompiled.h"
#endif*/

#include "StringUtils.h"
#include <iostream>
#include <istream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <ostream>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <string>
#include <exception>

#include "lsystemexception.h"

using namespace AP_LSystem;

/*
void StringUtils::LoadCfgFile(Configuration * cfg, const char * filename)
{
	fstream * configFile = new fstream(filename);
	if(configFile)
	{
		std::stringstream line;
		std::string id, s_line, temp;
		while(getline(*configFile,s_line))
		{
			line << s_line;
			while(line >> id)
			{
				if((id=="#configuration")&&(uncommentLine(line)))
				{
					if((id=="#endconfiguration")&&(uncommentLine(line)))
					{
						break;
					}
					
					while(getline(*configFile, s_line))
					{
						line << s_line;	
						if(line >> id)
						{
							if(id=="")
							{
							}
							// dodelat prazdny radek jen s komentarem
						}
					}

				}
				else
				{
					//if(configFile) configFile.close();
					//throw - chyba syntaxe
				}
			}
			line.clear();
		}
		configFile->close();
	}
	else 
	{
		//throw - soubor nelze otevrit
	}
}
*/
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
