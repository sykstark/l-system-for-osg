#include "precompiled.h"

#include "lsystem.h"
#include "configuration.h"
#include "lsfile.h"
#include "parstoch0lsystemgrammar.h"
#include "par2lsystemgrammar.h"
#ifdef _MSC_VER
#include "queryinterpret.h"
#endif

using namespace AP_LSystem;

LSystemGrammar::LSystemGrammar():_word(NULL)
{

}

void LSystemGrammar::loadFromFile( AbstractFile * file)
{
    this->_name = file->name();

    AbstractFile * subFile;
    AbstractGrammar * ls;

	// map for replacing grammar names in rules, axiom and homomorphism by their index
    std::map< string, string > lsystemSubstitute;
    // generate words of subgrammars
    vector<std::string>::iterator itGrammars = file->getSubsystems()->begin();
    for(; itGrammars != file->getSubsystems()->end(); itGrammars++)
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
            ls = new ParStoch0LSystemGrammar( subFile );
		else if( Par2LSystemGrammar::isCapable( subFile->type() ) )
            ls = new Par2LSystemGrammar( subFile );
        else
            throw ParsingException("non of L-systems fulfils the conditions");

        // make pair which bind name of lsystem with index in _subSystemsWords vector
        lsystemSubstitute.insert(make_pair<string, string >(subFile->name(),boost::lexical_cast<string>(_subSystemsWords.size())));
        _subSystemsWords.push_back( ls->translate() );
		
        delete subFile;
	}

	// replace all names in rules, homomorphism and axiom by their index
    if(!lsystemSubstitute.empty())
        file->substitute( lsystemSubstitute );

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
}

void LSystemGrammar::transcribeSubSystems()
{
    // process only if there is any subsystem
    if( _subSystemsWords.empty() )
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

		if( i >= _word->length() )
			break;

		parametersCnt = 0;
		_word->getParameters<int>( i, pParams, parametersCnt );

		if( parametersCnt != 1 )
			return;

        newWord->append( _subSystemsWords[ pParams[0] ] );

        //Log::write(newWord->toString());
    }

    if(_word)
        delete _word;

    _word = newWord;
	
}

LongString * LSystemGrammar::translate( )
{
    // TODO homomorphism

    this->transcribeSubSystems( );

    return _word;
}

bool LSystemGrammar::nextIteration( )
{
    int j=0;
    char * buffer = NULL;
    LongString * newWord = new LongString( );

#ifdef _MSC_VER
	// bind for ability to do queries
	// TODO only if queries is on
	QueryInterpret::get()->bindWord( newWord );
#endif

    multimap<char, Rule>::iterator * pRuleIt;
    pair<multimap<char, Rule>::iterator, multimap<char, Rule>::iterator > result;

    double parameters[100];
    double * pParams = parameters; // parameters pointer

    for(unsigned int i = 0; i < _word->length(); i++ )
    {
        // mozna dodat kontrolu estli jde o pismeno
        result = _rules.equal_range( (*_word)[i]);

        // not found
        if( result.first == result.second )
        {
            j = i;
            buffer = _word->getSymbol(i);
            if(buffer)
				newWord->append(buffer,i-j+1);
        }
        // found
        else
		{
            pRuleIt = selectRule( result.first, result.second, _word, i, pParams );

			if(pRuleIt)
			{
				generateSuccessor( newWord, *pRuleIt, pParams );
				delete pRuleIt;
			}
			else
			{
				j = i;
				buffer = _word->getSymbol(i);
				if(buffer)
					newWord->append(buffer,i-j+1);
			}
        }

        //Log::write(newWord->toString());
    }

    if(_word)
        delete _word;

    _word = newWord;

	processCutSymbol();

    return true;
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

void LSystemGrammar::processCutSymbol( )
{
	LongString * newWord = new LongString( );
	unsigned int len;
	char * data = NULL;

	// TODO predelat ten cyklus...konci uprostred
	// TODO optimalizace - neni vzdy treba kopirovat
	// look for cut symbol
	for(unsigned int i = 0; i < _word->length(); i++ )
    {
		// get data before cut symbol 
		data = _word->getData( i, len, '%' );

		// no data
		if(data == NULL)
			return;

		// append data
		newWord->append( data, len );

		// end of string
		if( i >= _word->length() )
			break;

		// find end and skip symbols of rest of this branch
		i = _word->findMatchingRightBracket( i ) - 1;

        //Log::write(newWord->toString());
    }

    if(_word)
        delete _word;

    _word = newWord;
}

multimap<char, Rule>::iterator * LSystemGrammar::selectRule(multimap<char, Rule>::iterator & begin, 
															multimap<char, Rule>::iterator &,
															LongString *,
															unsigned int &,
															double *)
{
    return &begin;
}
