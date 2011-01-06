#ifndef RANDOMINDEX_H_
#define RANDOMINDEX_H_

#include <vector>
#include <cstdlib>
#include <ctime>
#include "configuration.h"

using namespace boost::program_options;

namespace AP_LSystem {
/**
  * Class for generating random index for stochastic rule selection
  */
class RandomIndex
{
private:
    std::vector<double> probabilities;  ///< probability factors
    double sum;                         ///< sum of all probabilities
public:
    /**
      * Initialize random generator
      */
	static void init()
	{
		const variable_value * seed = Configuration::get()->getProperty( "RandomSeed" );
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
      * Constructor
      */
    RandomIndex()
    {
		sum = 0;
    }

    /**
      * Add probability factor of rule.
      * @param p probability
      */
    void addProbability( double p )
    {
        probabilities.push_back( sum + p );
        sum += p;
    }

    /**
      * Returns random index according to probability factor
      * @return random index
      */
    unsigned int getRandomIndex()
    {
        double r = (static_cast<double>(rand() % 100)) * sum / 100.0f;

        for(std::vector<double>::iterator it = probabilities.begin(); it != probabilities.end()-1; it++)
        {
            if( *it >= r )
            {
                return it - probabilities.begin();
            }
        }

        return probabilities.size( ) - 1;
    }
};
}
#endif // RANDOMINDEX_H
