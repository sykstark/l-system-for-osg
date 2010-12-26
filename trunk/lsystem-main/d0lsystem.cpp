#include "precompiled.h"

#include "d0lsystem.h"
#include "log.h"

using namespace AP_LSystem;

D0LSystem::D0LSystem( AbstractFile * file) : LSystem( file )
{
    this->loadFromFile( file );
}

bool D0LSystem::transcribe(multimap<char, Rule> &rules)
{
    int j=0;
    char * buffer = NULL;
    LongString * newWord = new LongString( );

    multimap<char, Rule>::iterator pRuleIt;

    for(unsigned int i = 0; i < m_Word->length(); i++ )
    {
		Log::get()->incModuleCounter();
        // mozna dodat kontrolu estli jde o pismeno
        pRuleIt = rules.find( (*m_Word)[i]);

        // not found
        if( pRuleIt == rules.end() )
        {
            j = i;
            buffer = m_Word->getSymbol(i);
            if(buffer)
                newWord->append(buffer,i-j+1);
        }
        // found
        else
        {
			Log::get()->incRewritingCounter();
            // add successor
            newWord->append( (*pRuleIt->second.staticStrings.begin())->str,
                             (*pRuleIt->second.staticStrings.begin())->length );
        }
    }

    if(m_Word)
        delete m_Word;

    m_Word = newWord;

//    processCutSymbol();

    return true;
}

void D0LSystem::processPredecessor(Rule & r, string * rule, string::iterator & it)
{
    // process a rule
    // example: A->BC

    // predecessor
    // example: A
    r.strictPredecessor = *it++;

    // transcryption sign
    // example: ->
    if( (*it++ != '-') || (*it++ != '>') )
    {
        throw ParsingException("Symbol \'->\' was expected!");
    }
}

void D0LSystem::processRuleSuccessor(Rule & r, string * rule, string::iterator & it)
{
    // look for opening bracket - end of static string
    //		each rule has to start with static string
    r.addStaticString(rule, it);

    // insert new rule into map with rules
    this->m_Rules.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
}

void D0LSystem::processHomomorphismSuccessor(Rule & r, string * hom, string::iterator & it)
{
    // homomorphism successor
    // example: +F(x)
    r.addStaticString( hom, it );

    // insert new rule into map with rules
    this->m_Homomorphisms.insert(std::make_pair< char, Rule >(r.strictPredecessor, r ));
}
