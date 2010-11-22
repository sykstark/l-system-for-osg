#include "precompiled.h"

#include "lsystem.h"
#include "configuration.h"
#include "lsfile.h"
#include "parstoch0lsystem.h"
#include "par2lsystem.h"
#include "abstractgenerator.h"
#ifdef _MSC_VER
#include "xmlfile.h"
#include "queryinterpret.h"
#endif

using namespace AP_LSystem;

LSystem::LSystem():_word(NULL)
{

}

void LSystem::loadFromFile( AbstractFile * file)
{
    this->_name = file->name();

    AbstractFile * subFile;
    AbstractLSystem * ls;

	// map for replacing l-system names in rules, axiom and homomorphism by their index
    std::map< string, string > lsystemSubstitute;
    // generate words of sublsystems
    vector<std::string>::iterator itLSystems = file->getSubsystems()->begin();
    for(; itLSystems != file->getSubsystems()->end(); itLSystems++)
    {
        // determine extension of file
        unsigned int pos = itLSystems->rfind( '.' );
        if( pos == std::string::npos )
        {
            return;
        }
        std::string ext = itLSystems->substr( pos + 1, std::string::npos );

        // load file depending on extension
        if( ext == "ls" )
        {
            subFile = new LSFile;
        }
#ifdef _MSC_VER
		else if( ext == "xml" )
		{
			subFile = new XmlFile;
		}
#endif
        else
        {
            throw FileException("unknown extension: " + ext);
        }

        // open file
        subFile->open(*itLSystems);

        // choose best L-system algorithm for word generation
        ls = AbstractGenerator::createLSystem( subFile );

        // make pair which bind name of lsystem with index in _subSystemsWords vector
        lsystemSubstitute.insert(std::make_pair<string, string >(subFile->name(),boost::lexical_cast<string>(_subSystemsWords.size())));
        _subSystemsWords.push_back( ls->translate() );
		
        delete subFile;
	}

	// replace all names in rules, homomorphism and axiom by their index
    if(!lsystemSubstitute.empty())
        file->substitute( lsystemSubstitute );

    // initialize word inside l-system and load an axiom into
    this->setAxiom( file->getAxiom() );

    // process rule strings - convert to instances of Rule class
    vector<std::string>::iterator itRules = file->getRules()->begin();
    for(; itRules != file->getRules()->end(); itRules++)
    {
        this->addRule( &*itRules );
    }

	// process homomoprhism strings - convert to instances of Rule class
	itRules = file->getHomomorphisms()->begin();
	for(; itRules != file->getHomomorphisms()->end(); itRules++)
    {
		this->addHomomorphism( &*itRules );
    }

	const variable_value * ignoreVar = Configuration::get()->getProperty( _name, "Ignore" );
	if( ignoreVar )
	{
		ignore = ignoreVar->as<std::string>();
	}

	unsigned initIteration = Configuration::get()->getProperty( _name, "Iteration" )->as<unsigned>();

	for( unsigned iter = 0; iter < initIteration; iter++ )
    {
        this->nextIteration();
    }
}

void LSystem::setAxiom(std::string & axiom)
{
    if(_word) delete _word;
    _word = new LongString( );
    _word->append('$');
    _word->append( static_cast<unsigned char>(Configuration::get()->getLSystemIndex( this->_name )) );
    _word->convertFromString( &axiom );
    _word->append('$');
}

void LSystem::transcribeSubSystems()
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

	// look for subsystems
	for(unsigned int i = 0; i < _word->length(); i++ )
    {
		// subsystems are indetified by $ sign - get all data before $
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

LongString * LSystem::translate( )
{
    // use all homomorphism rules
    this->transcribe( _homomorphisms );
    // insert all sub L-systems
    this->transcribeSubSystems( );
    // return final word
    return _word;
}

bool LSystem::nextIteration( )
{
    // increment the iteration number
    _iteration++;
//	vrecko::logger.debugLog("Processing %d. iteration...", _iteration );
    // use rules and generate new word by transcriptions
    return this->transcribe( _rules );
}

bool LSystem::transcribe(multimap<char, Rule> &rules)
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
    std::pair<multimap<char, Rule>::iterator, multimap<char, Rule>::iterator > result;

    double parameters[100];
    double * pParams = parameters; // parameters pointer

    for(unsigned int i = 0; i < _word->length(); i++ )
    {
        // mozna dodat kontrolu estli jde o pismeno
        result = rules.equal_range( (*_word)[i]);

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

void LSystem::addRule( std::string * rule )
{
    Rule r;
    std::string::iterator it = rule->begin();
    processPredecessor( r, rule, it );
    processRuleSuccessor( r, rule, it );
}

void LSystem::addHomomorphism( std::string * hom )
{
    Rule h;
    std::string::iterator it = hom->begin();
    processPredecessor( h, hom, it );
    processHomomorphismSuccessor( h, hom, it );
}

void LSystem::generateSuccessor(LongString * word, multimap<char, Rule>::iterator & it, double * parameters)
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
        word->append( (*dynStrIt)->Eval( parameters ) );
    }
    // pridani posledniho statickeho retezce
    word->append( (*stStrIt)->str, (*stStrIt)->length );
}

void LSystem::processCutSymbol( )
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

multimap<char, Rule>::iterator * LSystem::selectRule(multimap<char, Rule>::iterator & begin, 
															multimap<char, Rule>::iterator &,
															LongString *,
															unsigned int &,
															double *)
{
    return &begin;
}
