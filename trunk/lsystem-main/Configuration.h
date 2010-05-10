#pragma once

namespace AP_LSystem {
class Configuration
{
public:
	
	bool loadCfgFile(string);
	void setProperty(const string &, const double &);
	bool getProperty(const string &, double &);
	static Configuration * get();
	
private:
	Configuration();
	static Configuration* config;
	~Configuration();

	
	static inline bool uncomment(stringstream&);
	map<string, double> properties; 
};
}