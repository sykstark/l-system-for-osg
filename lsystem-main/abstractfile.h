#ifndef ABSTRACTFILE_H_
#define ABSTRACTFILE_H_

#include <string>
#include <vector>
#include <map>
#include "configuration.h"
#include "lsystemexception.h"

using std::string;

namespace AP_LSystem {
/**
  * Abstract class for file types.
  */
class AbstractFile
{
protected:
    unsigned int m_Type;                    ///< type of loaded L-system
    string m_Name;                          ///< name of loaded L-system
    string m_Axiom;                         ///< loaded axiom

    std::vector<string> m_Rules;            ///< container with loaded rules strings
    std::vector<string> m_Homomorphisms;    ///< container with loaded homomorphism rules
    std::vector<string> m_Subsytems;        ///< container with subsystem's filenames

    /**
      * Type of L-system is composed by binary adding of some capabilities. This
      * method binary adds new capability.
      * List of capability strings:
      * - 0L
      * - 1LL
      * - 1LR
      * - 2L
      * - IL
      * - DETERMINISTIC
      * - STOCHASTIC
      * - PARAMETRIC
      * @param type string with type to add
      */
	void addType( std::string & type )
	{
		if(type == "0L")
		{
            m_Type |= LS_0L;
		}
		else if(type == "1LL")
		{
            m_Type |= LS_1LL;
		}
		else if(type == "1LR")
		{
            m_Type |= LS_1LR;
		}
		else if(type == "2L")
		{
            m_Type |= LS_2L;
		}
        else if(type == "IL")
		{
            m_Type |= LS_IL;
		}
		else if(type == "DETERMINISTIC")
		{
            m_Type |= LS_DETERMINISTIC;
		}
		else if(type == "STOCHASTIC")
		{
            m_Type |= LS_STOCHASTIC;
		}
		else if(type == "PARAMETRIC")
		{
            m_Type |= LS_PARAMETRIC;
		}
		else
		{
			throw ParsingException("unknown lsystem type");
		}
    }
public:
    /**
      * Constructor - inicialize L-system type to zero
      */
    AbstractFile(): m_Type(0){}

    /**
      * Pure method for opening file.
      * @param filename filename of file
      */
    virtual void open( std::string & filename) = 0;

    /**
      * Substitute all occurences of map's keys by their values in axiom,
      * rules and homomorphisms.
      * @param pairs map with substitution pairs
      */
    void substitute(std::map<string, string> & pairs)
    {
        unsigned int i;
        std::vector<string>::iterator rule;
        std::map<std::string,std::string>::iterator subst;

        // substitute occurences in m_Axiom
        for(subst = pairs.begin(); subst != pairs.end(); subst++)
        {
            i=0;
            while((i = m_Axiom.find(subst->first,i))&&(i != std::string::npos))
            {
                m_Axiom.replace(i,subst->first.length(),subst->second);
                i += subst->second.length();
            }
        }

        // substitute occurences in m_Rules
        for(rule = m_Rules.begin();rule != m_Rules.end(); rule++)
        {
            for(subst = pairs.begin(); subst != pairs.end(); subst++)
            {
                i=0;
                while((i = rule->find(subst->first,i))&&(i != std::string::npos))
                {
                    rule->replace(i,subst->first.length(),subst->second);
                    i += subst->second.length();
                }
            }
        }

        for(rule = m_Homomorphisms.begin();rule != m_Homomorphisms.end(); rule++)
        {
            for(subst = pairs.begin(); subst != pairs.end(); subst++)
            {
                i=0;
                while((i = rule->find(subst->first,i))&&(i != std::string::npos))
                {
                    rule->replace(i,subst->first.length(),subst->second);
                    i += subst->second.length();
                }
            }
        }
    }

    /**
      * Get homomorphisms stored in std::vector.
      * @return homomorphisms in string format
      */
    virtual std::vector<string> * getHomomorphisms()
    {
        return &m_Homomorphisms;
    }

    /**
      * Get rules stored in std::vector.
      * @return rules in string format
      */
    virtual std::vector<string> * getRules()
    {
        return &m_Rules;
    }

    /**
      * Get filenames of all subsystems stored in std::vector.
      * @return subsystem filenames
      */
    virtual std::vector<string> * getSubsystems()
    {
        return &m_Subsytems;
    }

    /**
      * Get axiom - the initial word.
      * @return axiom
      */
    std::string & getAxiom()
    {
        return m_Axiom;
    }

    /**
      * Get type of L-system. Stored as bitmap of desired capabilities.
      * @return type of L-system
      */
    unsigned int type()
    {
        return m_Type;
    }

    /**
      * Returns name of L-system.
      * @return name of L-system
      */
    std::string & name()
    {
        return m_Name;
    }
};
}

#endif // ABSTRACTFILE_H
