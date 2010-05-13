#ifndef QT_APP
#include "precompiled.h"
#endif
#include "LSFileGrammar.h"
#include "StringUtils.h"
#include <fstream>

#include "fparser/fparser.hh"

using namespace AP_LSystem;

LSFileGrammar::LSFileGrammar( std::string * filename)
{
	this->loadFromFile(filename);		
}

LSFileGrammar::~LSFileGrammar(void)
{
}

void LSFileGrammar::loadFromFile( std::string * filename )
{
    std::fstream * grammarFile = new std::fstream(filename->c_str());
//	ifstream grammarFile(filename.c_str());
	if(!grammarFile)
	{	
		//throw - soubor nelze otevrit
	}	
	
	std::stringstream line;
	std::string id, s_line, temp;

//	while( 1 )
	{
		id = StringUtils::processLine( grammarFile, line );
		if(id=="#grammar")
		{
			line >> this->_name;
			while( true )
			{
				id = StringUtils::processLine( grammarFile, line);
				if(id=="#endgrammar" ) break;
	
				if(id=="#set")
				{
					string prop; // property name
					double value;  // value of property
					line >> prop >> value;
					Configuration::get()->setProperty(prop, value);  // property setting
				}
				else if(id=="#axiom")
				{
					this->setAxiom( &StringUtils::processLine( grammarFile, line) );
					if( StringUtils::processLine( grammarFile, line) != "#endaxiom")
					{
						// throw exception						
					}
				}
				else if(id=="#rules")
				{
					while(true)
					{	
						id = StringUtils::processLine( grammarFile, line);
						if( id!="#endrules" ) 
						{
							this->addRule( &id );
						}
						else
						{
							break;
						}
					}
				}
				else if(id=="#homomorphisms")
				{
					while(true)
					{	
						id = StringUtils::processLine( grammarFile, line);
						if( id!="#endhomomorphisms" ) 
						{
							this->addHomomorphism( &id );
						}
						else
						{
							break;
						}
					}
				}
				// dodelat prazdny radek jen s komentarem

			}
		}
		else
		{
			//if(configFile) configFile.close();
			//throw - chyba syntaxe
		}	
	}
	grammarFile->close();
	delete grammarFile;	
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
			// throw exception
			return;
		}
		else
		{
			r.variables = string( *rule, it - rule->begin(), pos);
		}

	}

	if( *it++ != ':' )
	{
		//throw exception
		return;
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
		// throw exception
		return;
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
			// exception
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

		str = string(start, end);
		r.staticStrings.push_back( StaticString( str.c_str(), end - start ) );
		it = ++end;
		// look for end bracket or another function
		do
		{
			pos  = rule->find_first_of( ",)", it - rule->begin() );
			end = rule->begin() + pos;
			
			FunctionParser * fp = new FunctionParser( );
			// parse string inside brackets
			if ( fp->Parse( string( it, end ), r.variables, false ) != -1 )
			{
				//throw exception
				return;
			}

			it = end + 1;
		} while ( rule->at(pos) !=')' );

		start = it;
	}
	// insert last part of rule - after last ')'
	str = string(start, rule->end( ));
	r.staticStrings.push_back( StaticString( str.c_str(), rule->end( ) - start ) );

	// insert new rule into map with rules
	this->_rules.insert(make_pair< char, Rule >(nonTerminal, r ));
}

void LSFileGrammar::setAxiom(const std::string * axiom)
{
	_axiom = *axiom;

	if(_word) delete _word;
	_word = new LongString( );
	_word->appendStr( axiom->c_str(), axiom->length() );
}

void LSFileGrammar::addHomomorphism(std::string * homomorphism)
{
	
}

void LSFileGrammar::nextIteration( )
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
			newWord->getParamaters( i, pParams, parCnt );
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
					newWord->appendStr( stStrIt->str, stStrIt->length );

					newWord->appendDouble( (*dynStrIt)->Eval( pParams ) );

				}
				// pridani posledniho statickeho retezce
				newWord->appendStr( stStrIt->str, stStrIt->length );

			}
		}
	}
	_word = newWord;
}

char * LSFileGrammar::translate()
{
	return _word->c_str( );
}
