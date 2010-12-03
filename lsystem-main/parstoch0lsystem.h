#ifndef PARSTOCH0LSYSTEM_H_
#define PARSTOCH0LSYSTEM_H_

#include "abstract0lsystem.h"

namespace AP_LSystem {
/**
  * Parametric stochastic context-free L-system
  */
class ParStoch0LSystem : public Abstract0LSystem
{
private:

    /**
      * Capabilities of this L-system
      */
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC | LS_STOCHASTIC | LS_PARAMETRIC;
public:
    ParStoch0LSystem( AbstractFile * );
    ParStoch0LSystem( const ParStoch0LSystem & );
    ParStoch0LSystem & operator=( const ParStoch0LSystem & );

    /**
      * When some module is loaded this method selects max one rule that will be used for transcription.
      * It search in range given by two iterators begin and end.
      * The range contain only rules that has common predecessor.
      * Current word and possition of predecessor must be passed. Parameters of selected
      * module will be stored to parameters array.
      * @param begin iterator to range of rules
      * @param end iterator to range of rules
      * @param word word that is currently processed
      * @param pos position of current predecessor in word
      * @param parameters array of parameters for transcription. Only double type.
      * @return pointer to iterator to rule that was selected. Returns NULL if no rule was selected.
      */
    virtual multimap<char, Rule>::iterator * selectRule(multimap<char, Rule>::iterator & begin,
                                                        multimap<char, Rule>::iterator & end,
                                                        LongString * word,
                                                        unsigned int & pos,
                                                        double * parameters);

    /**
      * Decides if L-system of passed type can by processed by parametric
      * stochastic context-free L-system
      * @param type type of L-system that will be compared with ParStoch0LSytem capabilities
      * @return true if it is capable to process this type
      */
    static bool isCapable( unsigned int type )
    {
        if( ( type & capabilities ) == type )
            return true;
        else
            return false;
    }

    /**
      * Virtual copy constructor
      */
    shared_ptr<AbstractLSystem> clone() const
    {
        return shared_ptr<AbstractLSystem>( new ParStoch0LSystem(*this) );
    }
};
}

#endif // PARSTOCH0LSYSTEM_H_
