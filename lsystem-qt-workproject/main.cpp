#include <QtCore/QCoreApplication>
#include <iostream>
#include "LSFileGrammar.h"

using namespace AP_LSystem;

int main(int argc, char *argv[])
{
    AbstractGrammar * grammar;

    std::string filename = "c:\\Documents and Settings\\mpa\\Dokumenty\\lsystem\\ls\\loadingTest02.ls";

    if( filename.empty() )
    {
            return 0;
    }

    unsigned int pos = filename.rfind( '.' );
    if( pos == std::string::npos )
    {
            return 0;
    }

    std::string ext = filename.substr( pos + 1, std::string::npos );

    if( ext == "ls" )
    {
            grammar = new LSFileGrammar( &filename );
            for( int i = 0; i < 17 ; i++)
            {
                    grammar->nextIteration();
            }

        cout << dynamic_cast<LSFileGrammar *>(grammar)->wordLength() << endl;
    }
}
