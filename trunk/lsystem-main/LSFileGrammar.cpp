#ifndef QT_APP
#include "precompiled.h"
#endif
#include "LSFileGrammar.h"
#include "StringUtils.h"
#include "lsfile.h"
#include "lsystemexception.h"

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
	bool parametric = false;
	unsigned int pos;
	std::string::iterator start, end, it = rule->begin();
	
	// process a rule
	// example: A(x,y)->B(x+1,y-2)

	// non terminal
	// example: A
	char nonTerminal = *it++;

	// parametric rule
	// example: (x,y)
	if( *it == '(' )
	{
		parametric = true;
		it++;
/*		do
		{
			variables.insert(*it++);
		}
		while( *it++ == ',' );
		if( *it++ != ')' ) 
		{
			//throw exception
		}*/

		pos = rule->find( ')', it - rule->begin() );
		if ( pos == std::string::npos )
		{
            throw ParsingException("missing ending bracket");
		}
		else
		{
            r.variables = string( it, rule->begin() + pos);
		}

        it = rule->begin() + pos + 1;

	}

	if( *it++ != ':' )
	{
        throw ParsingException("\':\' was expected");
	}

	if( *it != '*' )
	{
		FunctionParser * cond = r.condition;
		cond = new FunctionParser();

		// pridat konstanty
			
	}
	else
	{
		it++;
		r.condition = NULL;
	}

	if( (*it++ != '-') || (*it++ != '>') )
	{
        throw ParsingException("\'->\' was expected");
	}

	// look for opening bracket - end of static string
	//		each rule has to start with static string

	// slo by zrychlit testem na parametric - pridat rovnou cely retezec

	unsigned int closingPar; // position of ) for determination if the content inside ( ) has to be parsed
	
	start = it;

	unsigned int i;
	char c;
	string str;

	while( ( pos = rule->find( '(', it - rule->begin( ) ) ) != std::string::npos )
	{
		end = rule->begin() + pos;
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
        str = string(start, end + 1);
        r.staticStrings.push_back( StaticString( str.c_str(), end - start + 1 ) );

        cout << "static:" << str << endl;

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

            cout << "parser:" << string( it, end ) << endl;

            if( rule->at(pos) == ')' )
            {
                break;
            }
            else
            {
                r.staticStrings.push_back( StaticString(*end) );
                cout << "static:" << *end << endl;
            }
        }

        start = end;
	}
	// insert last part of rule - after last ')'
	str = string(start, rule->end( ));
	r.staticStrings.push_back( StaticString( str.c_str(), rule->end( ) - start ) );

    cout << "static:" << str << endl;

	// insert new rule into map with rules
	this->_rules.insert(make_pair< char, Rule >(nonTerminal, r ));
}

void LSFileGrammar::setAxiom(const std::string & axiom)
{
    //_axiom = axiom;

	if(_word) delete _word;
	_word = new LongString( );
    _word->appendStr( axiom.c_str(), axiom.length() );
}

void LSFileGrammar::addHomomorphism(std::string * homomorphism)
{
	
}

bool LSFileGrammar::nextIteration( )
{
	LongString * newWord = new LongString();
	multimap<char, Rule>::iterator ruleIt;
	pair<multimap<char, Rule>::iterator, multimap<char, Rule>::iterator > result;

	vector<StaticString>::iterator stStrIt;
	vector<FunctionParser*>::iterator dynStrIt;

	double parameters[100];
	double * pParams = parameters; // parameters pointer
	int parCnt = 0; // parameters counter

	for(unsigned int i = 0; i < _word->length(); i++ )
	{
		// mozna dodat kontrolu estli jde o pismeno
		result = _rules.equal_range( (*_word)[i]);

		
		if( result.first == result.second )
		{
			newWord->appendChar( (*_word)[i] );
		}
		else
		{
			parCnt = 0;
			// ziskej paramtery z pozice za pismenem
            if( !_word->getParamaters( i, pParams, parCnt ))
            {
                return false;
            }
			for( ruleIt = result.first; ruleIt != result.second; ruleIt++ )
			{
				if( ruleIt->second.condition != NULL )
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

                    cout << "appending static " << stStrIt->toString() <<endl;
					newWord->appendStr( stStrIt->str, stStrIt->length );
                    cout << "params " << pParams[0] << "," << pParams[1] << endl;
					newWord->appendDouble( (*dynStrIt)->Eval( pParams ) );

				}
				// pridani posledniho statickeho retezce
				newWord->appendStr( stStrIt->str, stStrIt->length );

			}
		}
	}
	_word = newWord;

    return true;
}

char * LSFileGrammar::translate()
{
	return _word->c_str( );
}
