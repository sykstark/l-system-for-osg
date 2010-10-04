/*#ifndef QT_APP
#include "precompiled.h"
#endif*/

#include <iostream>
#include <fstream>
#include "Configuration.h"

using namespace AP_LSystem;

Configuration *Configuration::config = 0;

Configuration::Configuration(void)
{
    description.add_options()
            ("texture", value<std::string>())
            ("default_angle", value<double>());
}

Configuration::~Configuration(void)
{
}

bool Configuration::loadCfgFile(std::string filename)
{
    std::ifstream ifs(filename.c_str());

    if(ifs)
    {
        store(parse_config_file(ifs, description), globalProperties);
        return true;
    }
    else
    {
        return false;
    }
}

Configuration * Configuration::get()
{
	if(!config) config = new Configuration;

	return config;
}

void Configuration::setProperty(const std::string &property)
{
    std::stringstream stream;
    stream << property;

    store(parse_config_file( stream, description), globalProperties );
    notify(globalProperties);
}

void Configuration::setProperty(const std::string &grammarID, const std::string &property)
{
    std::stringstream stream;
    stream << property;

    store(parse_config_file( stream, description), grammarProperties[ grammarID ] );
    notify(grammarProperties[ grammarID ]);
}

const variable_value * Configuration::getProperty(const std::string & name)
{
    if(!globalProperties.count(name))
	{
        return NULL;
	}
	else
    {
        return &globalProperties[name];
	}
}

const variable_value * Configuration::getProperty(const std::string &grammarID, const std::string & name)
{
    if(grammarProperties.count(grammarID))
    {
        if(grammarProperties[grammarID].count(name))
        {
            return &grammarProperties[grammarID][name];
        }
    }
    if(globalProperties.count(name))
    {
        return &globalProperties[name];
    }
    else
    {
        return NULL;
    }
}

vector<std::string> Configuration::getGrammarNames()
{
	std::vector<std::string> names;
	std::map<std::string, variables_map>::iterator it;
	for(it = grammarProperties.begin(); it != grammarProperties.end(); it++)
	{
		names.push_back( it->first );
	}

	return names;
}

