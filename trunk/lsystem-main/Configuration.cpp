#ifndef QT_APP
#include "precompiled.h"
#endif

#include "Configuration.h"

using namespace AP_LSystem;

Configuration *Configuration::config = 0;

Configuration::Configuration(void)
{
}

Configuration::~Configuration(void)
{
}

bool Configuration::loadCfgFile(std::string filename)
{
	//TODO try 
//	FileLoaders::LoadCfgFile(config, filename.c_str());

	//catch

	return true;

}

Configuration * Configuration::get()
{
	if(!config) config = new Configuration;

	return config;
}

void Configuration::setProperty(const std::string &property, const double &value)
{
    globalProperties[property] = value;
}

void Configuration::setProperty(const std::string &grammarID, const std::string &name, const double &value)
{
    grammarProperties[ grammarID ][ name ] = value;
}

bool Configuration::getProperty(const std::string & name, double & value)
{
    if(globalProperties.find(name)==globalProperties.end())
	{
		value = 0.0;
		return false;
	}
	else
	{
        value = globalProperties[name];
		return true;
	}
}

bool Configuration::getProperty(const std::string &grammarID, const std::string & name, double & value)
{
    if(grammarProperties.find(grammarID)==grammarProperties.end())
    {
        value = 0.0;
        return false;
    }
    else
    {
        if(grammarProperties[grammarID].find(name) == grammarProperties[grammarID].end())
        {
            value = 0.0;
            return false;
        }
        else
        {
            value = grammarProperties[grammarID][name];
            return true;
        }
    }
}

