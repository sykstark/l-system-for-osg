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

void Configuration::setProperty(const std::string &name, const double &value)
{
	properties[name] = value;
}

bool Configuration::getProperty(const std::string & name, double & value)
{
	if(properties.find(name)==properties.end())
	{
		value = 0.0;
		return false;
	}
	else
	{
		value = properties[name];
		return true;
	}
}


bool Configuration::uncomment(std::stringstream& ss)
{
    std::string temp;
        if( ss >> temp && ( temp.size()<2 || ( temp[0]!='/' && temp[1]!='/' )))
	{
		return false;
	}
	else 
	{
		return true;
	}
}
