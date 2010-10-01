#ifdef _MSC_VER
#include "precompiled.h"
#endif

#include "windows.h"
#include "LSFileGrammar.h"
#include "StringUtils.h"
#include "lsfile.h"
#include "lsystemexception.h"
#include "boost/lexical_cast.hpp"

#include "fparser/fparser.hh"

using namespace AP_LSystem;

LSFileGrammar::LSFileGrammar( std::string * filename): _word(NULL)//, _axiom("")
{
	this->loadFromFile(filename);		
}

LSFileGrammar::~LSFileGrammar(void)
{
}

void LSFileGrammar::loadFromFile( std::string * filename )
{
    AbstractFile * file;

    if( filename->empty() )
    {
        return;
    }

    unsigned int pos = filename->rfind( '.' );
    if( pos == std::string::npos )
    {
        return;
    }

    std::string ext = filename->substr( pos + 1, std::string::npos );

    if( ext == "ls" )
    {
        file = new LSFile;
    }
    else
    {
        throw FileException("unknown extension: ");
    }

    file->open(filename);

    this->setAxiom( file->getAxiom() );

    vector<std::string>::iterator itRules = file->getRules()->begin();
    for(; itRules != file->getRules()->end(); itRules++)
    {
        this->addRule( &*itRules );
    }
}

void LSFileGrammar::addRule(std::string * rule)
{
    Rule r;
    std::string::iterator it = rule->begin();
	
	// process a rule
    // example: A(x,y):x<y->B(x+1,y-2):max(1,x)

	// non terminal
	// example: A
	char nonTerminal = *it++;

	// parametric rule
	// example: (x,y)
    r.processParameters(rule, it);

    // condition of rule
    // example: :x<y
    r.processCondition(rule, it);

    // transcryption sign
    // example: ->
	if( (*it++ != '-') || (*it++ != '>') )
	{
        throw ParsingException("Symbol \'->\' was expected!");
	}

	// look for opening bracket - end of static string
	//		each rule has to start with static string

	// slo by zrychlit testem na parametric - pridat rovnou cely retezec

    while( r.addStaticString(rule, it) )
    {
        r.addDynamicString(rule, it);
    }

    r.processProbabilityFactor(rule, it);

//    cout << "static:" << str << endl;

	// insert new rule into map with rules
	this->_rules.insert(make_pair< char, Rule >(nonTerminal, r ));
}

void LSFileGrammar::setAxiom(std::string & axiom)
{
	if(_word) delete _word;
    _word = new LongString( );
    _word->convertFromString( &axiom );

    cout << _word->toString() << endl;
}

void LSFileGrammar::addHomomorphism(std::string * hom)
{
    Rule r;
    std::string::iterator end, it = hom->begin();
    StaticString * pSS;

    // process homomorphism
    // example: A(x,y):x<y->+F(x):max(1,y)

    // non terminal
    // example: A
    char nonTerminal = *it++;

    // parametric rule
    // example: (x,y)
    r.processParameters(hom, it);

    // condition of rule
    // example: :x<y
    r.processCondition(hom, it);

    // transcryption sign
    // example: ->
    if( (*it++ != '-') || (*it++ != '>') )
    {
        throw ParsingException("Symbol \'->\' was expected!");
    }

    // homomorphism successor
    // example: +F(x)
    string str;

    end = hom->begin() + hom->find( ':', it-hom->begin() );
    pSS = new StaticString(string(it, end));
    r.staticStrings.push_back( pSS );

    // probability factor
    // example: :max(1,y)
    r.processProbabilityFactor(hom, it);

    // insert new rule into map with rules
    this->_homomorphisms.insert(make_pair< char, Rule >(nonTerminal, r ));
	
}

bool LSFileGrammar::nextIteration( )
{
    LongString * newWord = new LongString( );
	multimap<char, Rule>::iterator ruleIt;
	pair<multimap<char, Rule>::iterator, multimap<char, Rule>::iterator > result;

    vector<StaticString*>::iterator stStrIt;
	vector<FunctionParser*>::iterator dynStrIt;

	double parameters[100];
	double * pParams = parameters; // parameters pointer
	int parCnt = 0; // parameters counter

//    cout << "word: ";

	for(unsigned int i = 0; i < _word->length(); i++ )
	{
		// mozna dodat kontrolu estli jde o pismeno
		result = _rules.equal_range( (*_word)[i]);

        OutputDebugStringA(_word->toString().c_str());
		
		if( result.first == result.second )
		{
            newWord->appendChar( (*_word)[i], false );
//            cout << (*_word)[i] << " | ";
		}
		else
		{
			parCnt = 0;
			// ziskej paramtery z pozice za pismenem
            if( !_word->getParamaters( i, pParams, parCnt ))
            { 
                return false;
            }

            // randomly select rule and take probability into account
            if(result.first->second.probabilityFactor)
            {
                for( ruleIt = result.first; ruleIt != result.second; ruleIt++ )
                {
                  //  ruleIt->second.
                }
            }

			for( ruleIt = result.first; ruleIt != result.second; ruleIt++ )
			{
                if( ruleIt->second.condition)
				{
					if( ruleIt->second.condition->Eval( pParams ) == 0 )
						continue;
				}

				for( stStrIt= ruleIt->second.staticStrings.begin(), 
					dynStrIt = ruleIt->second.dynamicStrings.begin();
					dynStrIt != ruleIt->second.dynamicStrings.end();
					stStrIt++, dynStrIt++)
				{
					// pridani statickych a dynamickych retezcu do slova ( krome posledniho statickeho )
                    newWord->appendStr( (*stStrIt)->str, (*stStrIt)->length );
                    OutputDebugStringA(newWord->toString().c_str());
                    newWord->appendDouble( (*dynStrIt)->Eval( pParams ) );
                    OutputDebugStringA(newWord->toString().c_str());

//                    cout << stStrIt->toString() << " | " << (*dynStrIt)->Eval( pParams ) << " | ";

				}
				// pridani posledniho statickeho retezce
//                cout << stStrIt->toString();
                newWord->appendStr( (*stStrIt)->str, (*stStrIt)->length );

			}
		}
	}
//    cout << endl;
    if(_word)
        delete _word;

	_word = newWord;

    return true;
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

char * LSFileGrammar::translate()
{
	return _word->c_str( );
}
