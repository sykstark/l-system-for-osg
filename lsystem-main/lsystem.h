#ifndef LSYSTEM_H_
#define LSYSTEM_H_

#include "abstractlsystem.h"
#include "rule.h"
#include "longstring.h"

using std::multimap;
using std::vector;
using std::string;

namespace AP_LSystem {
class LSystem : public AbstractLSystem
{
protected:
    LongString * m_Word;
    multimap<char, Rule> m_Rules;
    multimap<char, Rule> m_Homomorphisms;
    vector<string> m_SubSystemsFilenames;
    vector< LongString *> m_SubSystemsWords;
    string m_Ignore;

    // Loading and parsing
    virtual void setAxiom(std::string &);
    virtual void addRule(std::string *);
    virtual void addHomomorphism(std::string *);
    virtual void processPredecessor( Rule &, string *, string::iterator & ) = 0;
    virtual void processRuleSuccessor( Rule &, string *, string::iterator &) = 0;
    virtual void processHomomorphismSuccessor( Rule &, string *, string::iterator &) = 0;

    // Generating
    /**
      * Transcription process with rules
      * @return true if succeeded
      */
    virtual bool nextIteration();

    /**
      * This method provides a transcription funkcionality. It substitutes all modules in word
      * by their successors.
      * @param rules container with rules
      * @return true if succeeded
      */
    virtual bool transcribe( multimap<char, Rule> & rules );

    /**
      * This method is called during each transcription. It selects one or no rule from
      * union of rules that have common predecessor. The selection procedure depends on type
      * of L-system.
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
      * Generates a successor of word
      */
	virtual void generateSuccessor( LongString *, multimap<char, Rule>::iterator &, double *);
public:
    LSystem(AbstractFile *);
    LSystem( const LSystem & );
    ~LSystem();

    LSystem & operator=( const LSystem & );

    virtual void loadFromFile( AbstractFile * );
    
    unsigned int wordLength() { return m_Word->length(); }
    virtual void transcribeSubSystems( );
    virtual LongString * translate( );
	virtual void processCutSymbol( );
};
}
#endif // LSYSTEM_H_
