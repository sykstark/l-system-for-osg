//#pragma once

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <sstream>
#include <map>
#include <boost/program_options.hpp>

using namespace boost::program_options;
using std::vector;
using std::string;

namespace AP_LSystem {
class Configuration
{
public:
    bool loadCfgFile(string);
    void setProperty(const string &);
    void setProperty(const string &, const string &);
    const variable_value * getProperty(const string &);
    const variable_value * getProperty(const string &, const string &);
	vector<string> getGrammarNames( );
	static Configuration * get();
	
private:
	Configuration();
	static Configuration* config;
	~Configuration();

    options_description description;
    variables_map globalProperties;
    std::map<std::string, variables_map> grammarProperties;
};
}

#endif
