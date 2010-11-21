#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <sstream>
#include <map>
#include <boost/program_options.hpp>

using namespace boost::program_options;
using std::vector;
using std::string;

namespace AP_LSystem {

enum LSystemCapabilities
{
    LS_0L               = 0x00000001,
    LS_1LL              = 0x00000002,
    LS_1LR              = 0x00000004,
    LS_2L               = 0x00000008,
    LS_kL               = 0x00000080,
    LS_DETERMINISTIC    = 0x00000100,
    LS_STOCHASTIC       = 0x00000200,
    LS_PARAMETRIC       = 0x00010000,
};

class Configuration
{
private:
    Configuration();
    static Configuration* config;
    ~Configuration();

    options_description description;
    variables_map globalProperties;
    std::map<std::string, int> lsystemNameMap;
    std::vector<variables_map> lsystemProperties;

public:
    bool loadCfgFile(string);
    void setProperty(const string &);
    void setProperty(const string &, const string &);
	void setProperty(const string &, const string &, const string &);
    const int getLSystemIndex( const string & );
	const int getLSystemCount();
    const variable_value * getProperty(const string &);
    const variable_value * getProperty(const string &, const string &);
	const variable_value * getProperty(const unsigned int, const string &);
	static Configuration * get();
};
}

#endif