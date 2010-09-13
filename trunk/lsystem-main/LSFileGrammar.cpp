#ifdef _MSC_VER
#include "precompiled.h"
#endif

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
/*
    while( ( pos = rule->find_first_of( "(:", it - rule->begin( ) ) ) != std::string::npos )
	{
		end = rule->begin() + pos;

        if( *end == ':' )
        {
            it = end;
            break;
        }

		closingPar = rule->find( ')', pos );
		
		if( closingPar == std::string::npos )
		{
            throw ParsingException("missing ending bracket");
		}
		
		// set i as position od char after (
		for( i = pos + 1; i < closingPar; i++ )
		{
			c = rule->at(i); 
			if( !( isdigit(c) || (c == '.') || (c == ' ') || (c == '-') || (c == '+') ) )
				break;
		}

		if( i == closingPar )
		{
			it = rule->begin( ) + closingPar + 1; 
			continue;
		}

        // add static string - with (
        pSS = new StaticString( string(start, end + 1) );
        r.staticStrings.push_back( pSS );

//        cout << "static:" << str << endl;

        //end++;
		// look for end bracket or another function
        while(true)
		{
            it = ++end;
			pos  = rule->find_first_of( ",)", it - rule->begin() );
			end = rule->begin() + pos;
			
			FunctionParser * fp = new FunctionParser( );
			// parse string inside brackets
			if ( fp->Parse( string( it, end ), r.variables, false ) != -1 )
			{
                throw ParsingException("parsing of expression error");
			}
            r.dynamicStrings.push_back( fp );

//            cout << "parser:" << string( it, end ) << endl;

            if( rule->at(pos) == ')' )
            {
                break;
            }
            else
            {                
                pSS = new StaticString(*end);
                r.staticStrings.push_back( pSS );
//                cout << "static:" << *end << endl;
            }
        }

        start = end;
    }
	// insert last part of rule - after last ')'
    if( pos == std::string::npos )
    {
        // without probability
        pSS = new StaticString(string(start, rule->end( )));
    }
    else
    {
        // with probability
        pSS = new StaticString(string(start, end));
    }
    r.staticStrings.push_back( pSS );*/

    r.processProbabilityFactor(rule, it);

//    cout << "static:" << str << endl;

	// insert new rule into map with rules
	this->_rules.insert(make_pair< char, Rule >(nonTerminal, r ));
}

void LSFileGrammar::setAxiom(std::string & axiom)
{
	if(_word) delete _word;
	_word = new LongString( );
    std::string::iterator begin=axiom.begin(),end;
    unsigned int pos;
    double par;
    while(true)
    {
        pos = axiom.find_first_of( "(,)", begin - axiom.begin( ) );
        if ( pos == std::string::npos )
        {
            _word->appendStr(string(begin,axiom.end()));
            return;
        }
        end = axiom.begin() + pos;
        std::string str(begin, end);

        switch(*end)
        {
        case ',':
        case ')':
            par = lexical_cast<double>(str);
            _word->appendDouble(par);
            break;
        case '(':
            _word->appendStr(str);
            break;
        default:
            throw ParsingException("Axiom parsing error");
        }
        begin = end+1;
    }
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
	LongString * newWord = new LongString();
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
                    newWord->appendDouble( (*dynStrIt)->Eval( pParams ) );

//                    cout << stStrIt->toString() << " | " << (*dynStrIt)->Eval( pParams ) << " | ";

				}
				// pridani posledniho statickeho retezce
//                cout << stStrIt->toString();
                newWord->appendStr( (*stStrIt)->str, (*stStrIt)->length );

			}
		}
	}
//    cout << endl;
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
