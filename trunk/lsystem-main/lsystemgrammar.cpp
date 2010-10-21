#include "precompiled.h"

#include "lsystemgrammar.h"
#include "configuration.h"

using namespace AP_LSystem;
LSystemGrammar::LSystemGrammar():_word(NULL)
{

}

void LSystemGrammar::loadFromFile( AbstractFile * file)
{
    this->_name = file->name();
    this->setAxiom( file->getAxiom() );

    vector<std::string>::iterator itRules = file->getRules()->begin();
    for(; itRules != file->getRules()->end(); itRules++)
    {
        this->addRule( &*itRules );
    }
}

void LSystemGrammar::setAxiom(std::string & axiom)
{
    if(_word) delete _word;
    _word = new LongString( );
    _word->appendChar('$', true);
    _word->appendUByte( Configuration::get()->getGrammarIndex( this->_name ) );
    _word->convertFromString( &axiom );
    _word->appendChar('$', false);

    Log::write(_word->toString());
    //Log::get(); // TODO
}

LongString * LSystemGrammar::translate( )
{
    // TODO homomorphism

    return _word;
}

void LSystemGrammar::generateSuccessor(LongString * word, multimap<char, Rule>::iterator & it, double * parameters)
{
    vector<StaticString*>::iterator stStrIt;
    vector<FunctionParser*>::iterator dynStrIt;

    for( stStrIt= it->second.staticStrings.begin(),
        dynStrIt = it->second.dynamicStrings.begin();
        dynStrIt != it->second.dynamicStrings.end();
        stStrIt++, dynStrIt++)
    {
        // pridani statickych a dynamickych retezcu do slova ( krome posledniho statickeho )
        word->appendStr( (*stStrIt)->str, (*stStrIt)->length );
        //Log::write(newWord->toString());
        word->appendDouble( (*dynStrIt)->Eval( parameters ) );
        //Log::write(newWord->toString());
    }
    // pridani posledniho statickeho retezce
    word->appendStr( (*stStrIt)->str, (*stStrIt)->length );
}

multimap<char, Rule>::iterator & LSystemGrammar::selectRule(multimap<char, Rule>::iterator & begin, multimap<char, Rule>::iterator &)
{
    return begin;
}
