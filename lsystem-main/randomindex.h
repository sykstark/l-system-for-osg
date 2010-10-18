#ifndef RANDOMINDEX_H_
#define RANDOMINDEX_H_

#include <vector>
#include <cstdlib>
#include <ctime>

class RandomIndex
{
private:
    std::vector<double> probabilities;
    double sum;
public:
    RandomIndex()
    {
        srand( time( NULL ) );
    }

    void addProbability( double p )
    {
        probabilities.push_back( sum + p );
        sum += p;
    }

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

#endif // RANDOMINDEX_H
