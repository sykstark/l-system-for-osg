#pragma once

#include <sstream>
#include <map>
#include <boost/program_options.hpp>

using namespace boost::program_options;

namespace AP_LSystem {
class Configuration
{
public:
	
    bool loadCfgFile(std::string);
    void setProperty(const std::string &);
    void setProperty(const std::string &, const std::string &);
    const variable_value * getProperty(const std::string &);
    const variable_value * getProperty(const std::string &, const std::string &);
	static Configuration * get();
	
private:
	Configuration();
	static Configuration* config;
	~Configuration();

    //std::map<std::string, double> globalProperties;
    //std::map<std::string, std::map< std::string, double > > grammarProperties;

    options_description description;
    variables_map globalProperties;
    std::map<std::string, variables_map> grammarProperties;
};
}
