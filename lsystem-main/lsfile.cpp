#include <fstream>

#include "lsfile.h"
#include "StringUtils.h"
#include "lsystemexception.h"

using namespace std;
using namespace AP_LSystem;

LSFile::LSFile()
{
}

void LSFile::open(std::string & filename)
{
    std::fstream * file = new std::fstream(filename.c_str());

    if(!file)
    {
        throw FileException( "file " + filename + " cannot be opened" );
    }

    std::stringstream line;
    std::string id, emptyString;

    id = StringUtils::processLine( file, line );
    if(id=="#grammar")
    {
        line >> _name;
        while( true )
        {
            id = StringUtils::processLine( file, line);
            if(id=="#endgrammar" ) break;

            if(id=="#set")
            {
                string prop; // property
                line >> prop;
                if( prop == "" )
                {
                    throw ParsingException("Bad format of #set");
                }
                Configuration::get()->setProperty(_name, prop);  // property setting
            }
            else if(id=="#include")
            {
                string grammar; // property
                line >> grammar;
                if( grammar == "" )
                {
                    throw ParsingException("Bad format of #include");
                }
                grammars.push_back( grammar );
            }
            else if(id=="#define")
            {
                std::string definition, value;
                line >> definition >> value;
                if( (definition == "" ) || (value == ""))
                {
                    throw ParsingException("Bad format of #define");
                }
                defines.insert(make_pair<std::string, std::string>(definition, value));
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
                processType( line.str( ) );
            }
            else if(id=="#axiom")
            {
                this->axiom = StringUtils::processLine( file, line);

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
                        this->homomorhisms.push_back( id );
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

        processDefines();
    }
    else
    {
        if(file) file->close();
        throw ParsingException("#grammar not found");
    }
    file->close();
    delete file;
}

// nahradit vsechny vyskyty maker hodnotami
void LSFile::processDefines()
{
    unsigned int i;
    vector<string>::iterator rule;
    std::map<std::string,std::string>::iterator def;
    for(rule = rules.begin();rule != rules.end(); rule++)
    {
        for(def = defines.begin(); def != defines.end(); def++)
        {
            i=0;
            while((i = rule->find(def->first,i))&&(i != std::string::npos))
            {
                rule->replace(i,def->first.length(),def->second);
                i += def->first.length();

            }
        }
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
            throw ParsingException("error in parsing of grammar type");
        }
    }

    pos = 0;
    string type;
    while( true )
    {
        end = str.find('|', pos);
        if (end == string::npos )
            break;

        type = str.substr( pos, pos - end);
        if(type == "0L")
        {
            _type |= LS_0L;
        }
        else if(type == "1L")
        {
            _type |= LS_1L;
        }
        else
        {
            throw ParsingException("unknown grammar type");
        }

        pos = end + 1;
    }

}
