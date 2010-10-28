#include "precompiled.h"

#include "lsystemgrammar.h"
#include "configuration.h"
#include "lsfile.h"
#include "parstoch0lsystemgrammar.h"

using namespace AP_LSystem;
LSystemGrammar::LSystemGrammar():_word(NULL)
{

}

void LSystemGrammar::loadFromFile( AbstractFile * file)
{
    this->_name = file->name();

    AbstractFile * subFile;
    AbstractGrammar * grammar;

    // generate words of subgrammars
    vector<std::string>::iterator itGrammars = file->getGrammarFileNames()->begin();
    for(; itGrammars != file->getGrammarFileNames()->end(); itGrammars++)
    {
        // determine extension of file
        unsigned int pos = itGrammars->rfind( '.' );
        if( pos == std::string::npos )
        {
            return;
        }
        std::string ext = itGrammars->substr( pos + 1, std::string::npos );

        // load file depending on extension
        if( ext == "ls" )
        {
            subFile = new LSFile;
        }
        else
        {
            throw FileException("unknown extension: ");
        }

        // open file
        subFile->open(*itGrammars);

        // choose best for word generation
        if( ParStoch0LSystemGrammar::isCapable( subFile->type() ) )
            grammar = new ParStoch0LSystemGrammar( subFile );
        else
            throw ParsingException("non of grammars fulfils the conditions");

        _subGrammarsWords.insert(make_pair<std::string, LongString * >(file->name(),grammar->translate()));

        delete subFile;
    }

    // initialize word inside grammar and load an axiom into
    this->setAxiom( file->getAxiom() );

    // process rule strings - convert to instances of Rule class
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

void LSystemGrammar::transcribeSubGrammars()
{
	LongString * newWord = new LongString( );
	unsigned int len;
	char * data = NULL;

	for(unsigned int i = 0; i < _word->length(); i++ )
    {
		data = _word->getData( i, len, '{' );
		if(data == NULL)
			return;
		newWord->appendData( data, len );


        Log::write(newWord->toString());
    }

    if(_word)
        delete _word;

    _word = newWord;
	
}

LongString * LSystemGrammar::translate( )
{
    // TODO homomorphism

	this->transcribeSubGrammars( );

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
