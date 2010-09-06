#include <fstream>

#include "lsfile.h"
#include "StringUtils.h"
#include "lsystemexception.h"

using namespace AP_LSystem;

LSFile::LSFile()
{
}

void LSFile::open(std::string * filename)
{
    std::fstream * file = new std::fstream(filename->c_str());

    if(!file)
    {
        throw FileException( "file " + *filename + " cannot be opened" );
    }

    std::stringstream line;
    std::string id, grammarID;


    id = StringUtils::processLine( file, line );
    if(id=="#grammar")
    {
        line >> grammarID;
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
                Configuration::get()->setProperty(grammarID, prop);  // property setting
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
