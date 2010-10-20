#include "precompiled.h"

#include "LSFileGrammar.h"

#include "lsystemexception.h"
#include "boost/lexical_cast.hpp"
#include "log.h"
#include "randomindex.h"

#include "fparser/fparser.hh"

using namespace AP_LSystem;

/*LSFileGrammar::LSFileGrammar( AbstractFile * file): _word(NULL)//, _axiom("")
{
    this->loadFromFile(file);
}*/

LSFileGrammar::~LSFileGrammar(void)
{
}








bool LSFileGrammar::getEndOfStatic(string * rule, string::iterator & begin, string::iterator & end)
{
    unsigned int pos = rule->find_first_of( "(:", begin - rule->begin( ) );
    if ( pos != std::string::npos )
    {
        end = rule->begin() + pos;

        if( *end == ':' )
        {
            begin = end;
            return false;
        }
        return true;
    }
    return false;

}


