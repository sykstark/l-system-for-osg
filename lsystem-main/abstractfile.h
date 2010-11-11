#ifndef ABSTRACTFILE_H
#define ABSTRACTFILE_H

#include <string>
#include <vector>
#include <map>

using std::string;

namespace AP_LSystem {
class AbstractFile
{
protected:
    unsigned int _type;
    string _name;
    string axiom;

    std::vector<string> rules;
    std::vector<string> homomorhisms;
    std::vector<string> subsytems;
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
        return &homomorhisms;
    }

    virtual std::vector<string> * getRules()
    {
        return &rules;
    }

    virtual std::vector<string> * getSubsystems()
    {
        return &subsytems;
    }

    virtual std::string & getAxiom()
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
