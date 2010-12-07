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
        if(file)
        {
            delete file;
        }
        throw FileException( "file " + filename + " cannot be opened" );
    }

    std::stringstream line;
    std::string id, emptyString;

    id = StringUtils::processLine( file, line );
    if(id=="#lsystem")
    {
        line >> m_Name;
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
                    if(file)
                    {
                        file->close();
                        delete file;
                    }
                    throw ParsingException("Bad format of #set");
                }

                try
                {
                    Configuration::get()->setProperty(m_Name, prop);  // property setting
                }
                catch( std::exception & e)
                {
                    if(file)
                    {
                        file->close();
                        delete file;
                    }
                    throw ParsingException( "Property error in L-system \"" + m_Name + "\": " + string(e.what()) );
                }
            }
            else if(id=="#include")
            {
                string lsystem; // property
                line >> lsystem;
                //line.str("");
                if( lsystem == "" )
                {
                    if(file)
                    {
                        file->close();
                        delete file;
                    }
                    throw ParsingException("Bad format of #include");
                }
                m_Subsytems.push_back( lsystem );
            }
            else if(id=="#define")
            {
                std::string definition, value;
                line >> definition >> value;
                if( (definition == "" ) || (value == ""))
                {
                    if(file)
                    {
                        file->close();
                        delete file;
                    }
                    throw ParsingException("Bad format of #define");
                }
                defines.insert(std::make_pair<std::string, std::string>(definition, value));
            }
            else if(id=="#type")
            {
                // example : ( 2L | STOCHASTIC | 1L | 0L | DETERMINISTIC )
                if( m_Type )
                {
                    if(file)
                    {
                        file->close();
                        delete file;
                    }
                    throw ParsingException("#type was already set");
                }
				std::string type;
				line >> type;
                if( type == "")
                {
                    if(file)
                    {
                        file->close();
                        delete file;
                    }
                    throw ParsingException("Bad format of #type");
                }
                processType( type );
            }
            else if(id=="#axiom")
            {
                m_Axiom = StringUtils::processLine( file, line);

                if( StringUtils::processLine( file, line) != "#endaxiom")
                {
                    if(file)
                    {
                        file->close();
                        delete file;
                    }
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
                        this->m_Rules.push_back( id );
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
                        this->m_Homomorphisms.push_back( id );
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if(id.length())
            {
                if(file)
                {
                    file->close();
                    delete file;
                }
                throw ParsingException("Unknown expression: " + id);
            }

			line >> emptyString;
			if( emptyString != "" )
			{
                if(file)
                {
                    file->close();
                    delete file;
                }
                // there is something left in the row
				throw ParsingException("Illegal redundant operator: " + emptyString);
			}

        }

		// substitute all occurences of defined macros by their values
		substitute( this->defines );
    }
    else
    {
        if(file)
        {
            file->close();
            delete file;
        }
        throw ParsingException("#lsystem not found");
    }
    if(file)
    {
        file->close();
        delete file;
    }
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

        addType( type );

		pos = end + 1;
    }

}
