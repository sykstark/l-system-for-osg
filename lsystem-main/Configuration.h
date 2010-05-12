#pragma once

#include <sstream>
#include <map>

namespace AP_LSystem {
class Configuration
{
public:
	
    bool loadCfgFile(std::string);
    void setProperty(const std::string &, const double &);
    bool getProperty(const std::string &, double &);
	static Configuration * get();
	
private:
	Configuration();
	static Configuration* config;
	~Configuration();

	
        static inline bool uncomment(std::stringstream&);
        std::map<std::string, double> properties;
};
}
