#ifndef QT_APP
#include "precompiled.h"
#endif

#include <fstream>

#include "lsfile.h"
#include "StringUtils.h"
#include "lsystemexception.h"

using namespace AP_LSystem;

LSFile::LSFile()
{
}

void LSFile::loadFromFile(std::string * filename)
{
    std::fstream * file = new std::fstream(filename->c_str());

    if(!file)
    {
        throw LSystemException( "TESTOVACI" );
        //throw - soubor nelze otevrit
    }

    std::stringstream line;
    std::string id, s_line, temp, grammarID;


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
                string prop; // property name
                double value;  // value of property
                line >> prop >> value;
                Configuration::get()->setProperty(grammarID, prop, value);  // property setting
            }
            else if(id=="#axiom")
            {
                this->axiom = StringUtils::processLine( file, line);

                if( StringUtils::processLine( file, line) != "#endaxiom")
                {
                        // throw exception
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
            // dodelat prazdny radek jen s komentarem

        }
    }
    else
    {
        //if(configFile) configFile.close();
        //throw - chyba syntaxe
    }
    file->close();
    delete file;
}
