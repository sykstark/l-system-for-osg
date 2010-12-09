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
      * Generates a successor of module by recurrent appending of rule's static strings and evaluated
      * expressions.
      * @param word strings will bee appended to this word
      * @param rule selected rule
      * @param params parameter values for expressions evaluation
      */
    virtual void generateSuccessor( LongString * word, multimap<char, Rule>::iterator & rule, double * params);
public:

    /**
      * Constructor
      */
    LSystem(AbstractFile *);

    /**
      * Copy constructor
      */
    LSystem( const LSystem & );

    /**
      * Destructor
      */
    ~LSystem();

    /**
      * Assignment operator
      */
    LSystem & operator=( const LSystem & );

    /**
      * Load L-system from file
      * @param file of L-system as instance of
    virtual void loadFromFile( AbstractFile * file );
    
    unsigned int wordLength() { return m_Word->length(); }
    virtual void transcribeSubSystems( );
    virtual LongString * translate( );
	virtual void processCutSymbol( );
};
}
#endif // LSYSTEM_H_
