#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H

#include <string>
#include <vector>
#include <map>
#include "configuration.h"
#include "lsystemexception.h"

using std::string;

namespace AP_LSystem {
class AbstractFile
{
protected:
    unsigned int _type;
    string _name;
    string axiom;

    std::vector<string> rules;
    std::vector<string> homomorphisms;
    std::vector<string> subsytems;

	void addType( std::string & type )
	{
		if(type == "0L")
		{
			_type |= LS_0L;
		}
		else if(type == "1LL")
		{
			_type |= LS_1LL;
		}
		else if(type == "1LR")
		{
			_type |= LS_1LR;
		}
		else if(type == "2L")
		{
			_type |= LS_2L;
		}
		else if(type == "kL")
		{
			_type |= LS_kL;
		}
		else if(type == "DETERMINISTIC")
		{
			_type |= LS_DETERMINISTIC;
		}
		else if(type == "STOCHASTIC")
		{
			_type |= LS_STOCHASTIC;
		}
		else if(type == "PARAMETRIC")
		{
			_type |= LS_PARAMETRIC;
		}
		else
		{
			throw ParsingException("unknown lsystem type");
		}
	}
public:
    AbstractFile(): _type(0){}
    virtual void open( std::string & ) = 0;

    // nahradit vsechny vyskyty maker hodnotami
    void substitute(std::map<string, string> & pairs)
    {
        unsigned int i;
        std::vector<string>::iterator rule;
        std::map<std::string,std::string>::iterator subst;

        // substitute occurences in axiom
        for(subst = pairs.begin(); subst != pairs.end(); subst++)
        {
            i=0;
            while((i = axiom.find(subst->first,i))&&(i != std::string::npos))
            {
                axiom.replace(i,subst->first.length(),subst->second);
                i += subst->first.length();
            }
        }

        // substitute occurences in rules
        for(rule = rules.begin();rule != rules.end(); rule++)
        {
            for(subst = pairs.begin(); subst != pairs.end(); subst++)
            {
                i=0;
                while((i = rule->find(subst->first,i))&&(i != std::string::npos))
                {
                    rule->replace(i,subst->first.length(),subst->second);
                    i += subst->first.length();
                }
            }
        }

        // TODO substitute occurences in homomorphisms
    }

    virtual std::vector<string> * getHomomorphisms()
    {
        return &homomorphisms;
    }

    virtual std::vector<string> * getRules()
    {
        return &rules;
    }

    virtual std::vector<string> * getSubsystems()
    {
        return &subsytems;
    }

    std::string & getAxiom()
    {
        return axiom;
    }

    unsigned int type()
    {
        return _type;
    }

    std::string & name()
    {
        return _name;
    }
};
}

#endif // ABSTRACTFILE_H
