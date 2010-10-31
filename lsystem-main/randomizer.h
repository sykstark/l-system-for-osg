#ifndef RANDOMIZER_H_
#define RANDOMIZER_H_

#include <vector>
#include <cstdlib>
#include <ctime>

namespace AP_LSystem {
class Randomizer
{
public:
	static void init()
	{
		srand( time( NULL ) );
	}

	static double get( unsigned int maxPercents )
	{
		return 1 + (((rand()%2)?(-1):(1)) * (static_cast<double>(rand() % (maxPercents*10)) / 1000.0f));
	}
};
}

#endif
