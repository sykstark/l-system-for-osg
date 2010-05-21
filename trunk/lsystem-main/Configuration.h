#pragma once

#include <sstream>
#include <map>

namespace AP_LSystem {
class Configuration
{
public:
	
    bool loadCfgFile(std::string);
    void setProperty(const std::string &, const double &);
    void setProperty(const std::string &, const std::string &, const double &);
    bool getProperty(const std::string &, double &);
    bool getProperty(const std::string &, const std::string &, double &);
	static Configuration * get();
	
private:
	Configuration();
	static Configuration* config;
	~Configuration();

    std::map<std::string, double> globalProperties;
    std::map<std::string, std::map< std::string, double > > grammarProperties;
};
}
