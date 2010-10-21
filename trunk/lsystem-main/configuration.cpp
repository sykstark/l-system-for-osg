#include "precompiled.h"

#include <iostream>
#include <fstream>
#include "configuration.h"

using namespace AP_LSystem;

Configuration *Configuration::config = 0;

Configuration::Configuration(void)
{
    description.add_options()
            ("iteration", value<unsigned int>())
			("contour_detail", value<unsigned int>())
            ("texture", value<std::string>())
            ("default_angle", value<double>())
			("default_length", value<double>())
			("default_radius", value<double>())
			("length_increment", value<double>())
			("turtle_type", value<std::string>());
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

const int Configuration::getGrammarIndex(const string & name)
{
    if( grammarNameMap.count(name))
    {
        return grammarNameMap[name];
    }
    else
        return -1;
}

const int Configuration::getGrammarCount()
{
	return grammarProperties.size();
}

void Configuration::setProperty(const std::string &prop)
{
    std::stringstream stream;
    stream << prop;

    store(parse_config_file( stream, description), globalProperties );
    notify(globalProperties);
}

void Configuration::setProperty(const std::string &grammarID, const std::string &prop)
{
    if( !grammarNameMap.count( grammarID ) )
    {
        // bind grammar name with index in grammarProperties vector
        grammarNameMap[grammarID] = grammarProperties.size();
        grammarProperties.push_back( variables_map() );
    }

    std::stringstream stream;
    stream << prop;

    store(parse_config_file( stream, description), grammarProperties[ grammarNameMap[grammarID] ] );
    notify(grammarProperties[ grammarNameMap[grammarID] ]);
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
    if(grammarNameMap.count(grammarID))
    {
        if(grammarProperties[grammarNameMap[grammarID]].count(name))
        {
            return &grammarProperties[grammarNameMap[grammarID]][name];
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

const variable_value * Configuration::getProperty(const unsigned int grammarIndex, const std::string & name)
{
	if(grammarIndex < grammarProperties.size( ))
    {
		if(grammarProperties[grammarIndex].count(name))
        {
            return &grammarProperties[grammarIndex][name];
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

/*vector<std::string> & Configuration::getGrammarNames()
{
	return grammarIDs;
}
*/
