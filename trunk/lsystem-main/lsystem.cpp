#include "precompiled.h"

#include "lsystem.h"
#include "configuration.h"
#include "lsfile.h"
#include "parstoch0lsystem.h"
#include "par2lsystem.h"
#include "abstractgenerator.h"
#include "log.h"
#ifdef _MSC_VER
#include "xmlfile.h"
#include "queryinterpret.h"
#endif

using namespace AP_LSystem;
using namespace std;

LSystem::LSystem( AbstractFile * file ):m_Word(NULL), m_Ignore("")
{
}

LSystem::LSystem( const LSystem & c ): AbstractLSystem( c ),
                                       m_Rules(c.m_Rules),
                                       m_Homomorphisms(c.m_Homomorphisms),
                                       m_SubSystemsFilenames(c.m_SubSystemsFilenames),
                                       m_Ignore(c.m_Ignore)
{
    for(vector< LongString *>::const_iterator it = c.m_SubSystemsWords.begin(); it != c.m_SubSystemsWords.end(); it++)
        m_SubSystemsWords.push_back( new LongString(**it) );

    m_Word = (c.m_Word)?( new LongString(*(c.m_Word)) ):NULL;
}

LSystem & LSystem::operator =( const LSystem & c)
{
    m_Word = new LongString(*(c.m_Word));
    m_Rules = c.m_Rules;
    m_Homomorphisms = c.m_Homomorphisms;
    m_SubSystemsFilenames = c.m_SubSystemsFilenames;
    for(vector< LongString *>::const_iterator it = c.m_SubSystemsWords.begin(); it != c.m_SubSystemsWords.end(); it++)
        m_SubSystemsWords.push_back( new LongString(**it) );
    m_Ignore = c.m_Ignore;
    return *this;
}

LSystem::~LSystem()
{
    if(m_Word)
        delete m_Word;
}

void LSystem::loadFromFile( AbstractFile * file)
{
    this->m_Name = file->name();

    AbstractFile * subFile;
    boost::shared_ptr<AbstractLSystem> ls;

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

        // make pair which bind name of lsystem with index in m_SubSystemsWords vector
        lsystemSubstitute.insert(std::make_pair<string, string >(subFile->name(),boost::lexical_cast<string>(m_SubSystemsWords.size())));
        m_SubSystemsWords.push_back( ls->translate() );
		
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

//	if(this->m_Rules.empty())
//		vrecko::logger.debugLog( "No rules loaded from L-system %s", this->_name );	

	// process homomoprhism strings - convert to instances of Rule class
	itRules = file->getHomomorphisms()->begin();
	for(; itRules != file->getHomomorphisms()->end(); itRules++)
    {
		this->addHomomorphism( &*itRules );
    }

    const variable_value * ignoreVar = Configuration::get()->getProperty( m_Name, "Ignore" );
	if( ignoreVar )
	{
		m_Ignore = ignoreVar->as<std::string>();
	}

    unsigned initIteration = Configuration::get()->getProperty( m_Name, "Iteration" )->as<unsigned>();

	for( unsigned iter = 0; iter < initIteration; iter++ )
    {
        this->nextIteration();
    }
}

void LSystem::setAxiom(std::string & axiom)
{
    if(m_Word) delete m_Word;
    m_Word = new LongString( );
    m_Word->append('$');
    m_Word->append( static_cast<unsigned char>(Configuration::get()->getLSystemIndex( this->m_Name )) );
    m_Word->convertFromString( &axiom );
    m_Word->append('$');
}

void LSystem::transcribeSubSystems()
{
    // process only if there is any subsystem
    if( m_SubSystemsWords.empty() )
		return;

	LongString * newWord = new LongString( );
	unsigned int len;
	char * data = NULL;
	int parameters[100];
	int * pParams = parameters;
	int parametersCnt = 0;

	// look for subsystems
	for(unsigned int i = 0; i < m_Word->length(); i++ )
    {
		// subsystems are indetified by $ sign - get all data before $
		data = m_Word->getData( i, len, '#' );

		// no data
		if(data == NULL)
			return;

		// append data before
		newWord->append( data, len );

		if( i >= m_Word->length() )
			break;

		parametersCnt = 0;
		m_Word->getParameters<int>( i, pParams, parametersCnt );

		if( parametersCnt != 1 )
			return;

        newWord->append( m_SubSystemsWords[ pParams[0] ] );

        //Log::write(newWord->toString());
    }

    if(m_Word)
        delete m_Word;

    m_Word = newWord;
	
}

LongString * LSystem::translate( )
{
	// backup and make a copy of current word
	LongString * original = new LongString( *m_Word );
    // use all homomorphism rules
    this->transcribe( m_Homomorphisms );
    // insert all sub L-systems
    this->transcribeSubSystems( );
	// swap words
	LongString * final = m_Word;
	m_Word = original;
    // return final word
    return final;
}

bool LSystem::nextIteration( )
{
    // increment the iteration number
    m_Iteration++;
//	vrecko::logger.debugLog("Processing %d. iteration...", m_Iteration );
    // use rules and generate new word by transcriptions
    return this->transcribe( m_Rules );
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

    for(unsigned int i = 0; i < m_Word->length(); i++ )
    {
        // mozna dodat kontrolu estli jde o pismeno
        result = rules.equal_range( (*m_Word)[i]);

        // not found
        if( result.first == result.second )
        {
            j = i;
            buffer = m_Word->getSymbol(i);
            if(buffer)
				newWord->append(buffer,i-j+1);
        }
        // found
        else
		{
            pRuleIt = selectRule( result.first, result.second, m_Word, i, pParams );

			if(pRuleIt)
			{
				generateSuccessor( newWord, *pRuleIt, pParams );
				delete pRuleIt;
			}
			else
			{
				j = i;
				buffer = m_Word->getSymbol(i);
				if(buffer)
					newWord->append(buffer,i-j+1);
			}
        }

        //Log::write(newWord->toString());
    }

    if(m_Word)
        delete m_Word;

    m_Word = newWord;

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
	for(unsigned int i = 0; i < m_Word->length(); i++ )
    {
		// get data before cut symbol 
		data = m_Word->getData( i, len, '%' );

		// no data
		if(data == NULL)
			return;

		// append data
		newWord->append( data, len );

		// end of string
		if( i >= m_Word->length() )
			break;

		// find end and skip symbols of rest of this branch
		i = m_Word->findMatchingRightBracket( i ) - 1;

        //Log::write(newWord->toString());
    }

    if(m_Word)
        delete m_Word;

    m_Word = newWord;
}

multimap<char, Rule>::iterator * LSystem::selectRule(multimap<char, Rule>::iterator & begin, 
															multimap<char, Rule>::iterator &,
															LongString *,
															unsigned int &,
															double *)
{
    return &begin;
}
