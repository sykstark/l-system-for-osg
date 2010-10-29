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

	// map for replacing grammar names in rules, axiom and homomorphism by their index
	std::map< string, string > grammarSubstitute;
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

		// make pair which bind name of grammar with index in _subGrammarWords vector
		grammarSubstitute.insert(make_pair<string, string >(subFile->name(),boost::lexical_cast<string>(_subGrammarsWords.size())));
		_subGrammarsWords.push_back( grammar->translate() );
		
        delete subFile;
    }

	// replace all names in rules, homomorphism and axiom by their index
	if(!grammarSubstitute.empty())
		file->substitute( grammarSubstitute );

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
    _word->append('$');
    _word->append( static_cast<unsigned char>(Configuration::get()->getGrammarIndex( this->_name )) );
    _word->convertFromString( &axiom );
    _word->append('$');

    Log::write(_word->toString());
    //Log::get(); // TODO
}

void LSystemGrammar::transcribeSubGrammars()
{
	// process only if there is any subgrammar
	if( _subGrammarsWords.empty() )
		return;

	LongString * newWord = new LongString( );
	unsigned int len;
	char * data = NULL;
	int parameters[100];
	int * pParams = parameters;
	int parametersCnt = 0;

	// look for subgrammars
	for(unsigned int i = 0; i < _word->length(); i++ )
    {
		// subgrammars are indetified by $ sign - get all data before $
		data = _word->getData( i, len, '#' );

		// no data
		if(data == NULL)
			return;

		// append data before
		newWord->append( data, len );
		_word->getParameters<int>( i, pParams, parametersCnt );

		if( parametersCnt != 1 )
			return;

		newWord->append( _subGrammarsWords[ pParams[0] ] );
		
		i++;	

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
        word->append( (*stStrIt)->str, (*stStrIt)->length );
        //Log::write(newWord->toString());
        word->append( (*dynStrIt)->Eval( parameters ) );
        //Log::write(newWord->toString());
    }
    // pridani posledniho statickeho retezce
    word->append( (*stStrIt)->str, (*stStrIt)->length );
}

multimap<char, Rule>::iterator & LSystemGrammar::selectRule(multimap<char, Rule>::iterator & begin, multimap<char, Rule>::iterator &)
{
    return begin;
}
