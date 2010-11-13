#include <fstream>
#include <sstream>

#include "lsfile.h"
#include "utils.h"
#include "lsystemexception.h"
#include "configuration.h"

using namespace AP_LSystem;

LSFile::LSFile()
{
}

void LSFile::open(std::string & filename)
{
    std::fstream * file = new std::fstream(filename.c_str());

	if(!file->is_open( ))
    {
        throw FileException( "file " + filename + " cannot be opened" );
    }

    std::stringstream line;
    std::string id, emptyString;

    id = StringUtils::processLine( file, line );
    if(id=="#lsystem")
    {
        line >> _name;
        while( true )
        {
            id = StringUtils::processLine( file, line);
            if(id=="#endlsystem" ) break;

            if(id=="#set")
            {
                string prop; // property
                line >> prop;
                if( prop == "" )
                {
                    throw ParsingException("Bad format of #set");
                }

                try
                {
                    Configuration::get()->setProperty(_name, prop);  // property setting
                }
                catch( std::exception & e)
                {
					throw ParsingException( "Property error in L-system \"" + _name + "\": " + string(e.what()) );
                }
            }
            else if(id=="#include")
            {
                string lsystem; // property
                line >> lsystem;
                //line.str("");
                if( lsystem == "" )
                {
                    throw ParsingException("Bad format of #include");
                }
                subsytems.push_back( lsystem );
            }
            else if(id=="#define")
            {
                std::string definition, value;
                line >> definition >> value;
                if( (definition == "" ) || (value == ""))
                {
                    throw ParsingException("Bad format of #define");
                }
                defines.insert(std::make_pair<std::string, std::string>(definition, value));
            }
            else if(id=="#type")
            {
                // ( 2L | STOCHASTIC | 1L | 0L | DETERMINISTIC )
                if( _type )
                {
                    throw ParsingException("#type was already set");
                }
				std::string type;
				line >> type;
                if( type == "")
                {
                    throw ParsingException("Bad format of #type");
                }
                processType( type );
            }
            else if(id=="#axiom")
            {
                axiom = StringUtils::processLine( file, line);

                if( StringUtils::processLine( file, line) != "#endaxiom")
                {
                    throw ParsingException("#endaxiom not found");
                }
            }
            else if(id=="#rules")
            {
                while(true)
                {
                    id = StringUtils::processLine( file, line);
                    if( id!="#endrules" )
                    {
                        this->rules.push_back( id );
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(id=="#homomorphisms")
            {
                while(true)
                {
                    id = StringUtils::processLine( file, line);
                    if( id!="#endhomomorphisms" )
                    {
                        this->homomorphisms.push_back( id );
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(id.length())
            {
                throw ParsingException("Unknown expression: " + id);
            }
            // dodelat prazdny radek jen s komentarem

			line >> emptyString;
			if( emptyString != "" )
			{
				// v radu neco zbylo navic
				throw ParsingException("Illegal redundant operator: " + emptyString);
			}

        }

		// substitute all occurences of defined macros by their values
		substitute( this->defines );
    }
    else
    {
        if(file) file->close();
        throw ParsingException("#lsystem not found");
    }
    file->close();
    delete file;
}



void LSFile::processType( std::string str )
{
    std::string::size_type pos, end;
    while( true )
    {
        pos = str.find(' ');
        if( pos != string::npos )
            str.erase( pos, 1 );
        else
            break;
    }
    if(str[0] == '(')
    {
        str.erase( 0, 1);
        if( str[str.length()-1] == '(' )
            str.erase(str.length()-1, 1);
        else
        {
            throw ParsingException("error in parsing of lsystem type");
        }
    }

    pos = 0;
    string type;
	while( pos < str.length() )
    {
        end = str.find('|', pos);
		if (end == string::npos )
		{
			end = str.length();
		}
		
		type = str.substr( pos, end - pos);

        if(type == "0L")
        {
            _type |= LS_0L;
        }
        else if(type == "1LL")
        {
            _type |= LS_1LL;
        }
		else if(type == "1LR")
        {
			_type |= LS_1LR;
        }
		else if(type == "2L")
        {
			_type |= LS_2L;
        }
		else if(type == "kL")
        {
            _type |= LS_kL;
        }
		else if(type == "DETERMINISTIC")
        {
			_type |= LS_DETERMINISTIC;
        }
		else if(type == "STOCHASTIC")
        {
            _type |= LS_STOCHASTIC;
        }
		else if(type == "PARAMETRIC")
        {
            _type |= LS_PARAMETRIC;
        }
        else
        {
            throw ParsingException("unknown lsystem type");
        }

		pos = end + 1;
    }

}
