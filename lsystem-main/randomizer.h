#ifndef RANDOMIZER_H_
#define RANDOMIZER_H_

#include <vector>
#include <cstdlib>
#include <ctime>
#include "configuration.h"

using namespace boost::program_options;

namespace AP_LSystem {
/**
  * Library class for generating random numbers
  */
class Randomizer
{
public:
    /**
      * Initialize random generator
      */
	static void init()
	{
		const variable_value * seed = Configuration::get()->getProperty( "AngleFluctuationSeed" );
		if( seed )
		{
			srand( seed->as<unsigned int>() );
		}
		else
		{
			srand( time( NULL ) );
		}
	}

    /**
      * Returns random double number in percents with one decimal
      * @param maxPercents maximum percents. Sets random number range as <-maxPercets, maxPercents>.
      * @return random number
      */
	static double get( unsigned int maxPercents )
	{
		return 1 + (((rand()%2)?(-1):(1)) * (static_cast<double>(rand() % (maxPercents*10)) / 1000.0f));
	}

    /**
      * Returns random double number with one decimal. This method is in form for using with
      * FunctionParser.
      * @param max maximum number. Sets random number range as <-max, max>.
      * @return random number
      */
    static double get( const double * max )
    {
        return get( static_cast<unsigned>(abs(*max)) );
    }
};
}

#endif
