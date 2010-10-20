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
    _word->appendChar('?', true);
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
