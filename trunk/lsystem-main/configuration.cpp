#include "precompiled.h"

#include <iostream>
#include <fstream>
#include "configuration.h"
#include "lsystemexception.h"

using namespace AP_LSystem;

Configuration *Configuration::config = 0;

Configuration::Configuration(void)
{
    // description list of all properties
    description.add_options()
            ("iteration", value<unsigned int>())
			("contour_detail", value<unsigned int>())
            ("diffuse_texture", value<std::string>())
			("texture_s_repeating", value<unsigned int>())
			("gravitropism_elasticity", value<double>())
			("tropism_elasticity", value<double>())
			("tropism_angle", value<double>())
			("tropism_vector", value<std::string>())
            ("default_angle", value<double>())
			("default_length", value<double>())
			("default_radius", value<double>())
			("angle_multiplier", value<double>())
			("length_multiplier", value<double>())
			("radius_multiplier", value<double>())
			("debug_geometry_scale", value<double>())
			("minimize_twist", value<unsigned int>())
			("degrees_to_radians", value<unsigned int>())
			("draw_debug_geometry", value<unsigned int>())
			("angle_variance", value<unsigned int>())
			("separate_geometry_for_translucent", value<unsigned int>())			
			("turtle_type", value<std::string>());
}

Configuration::~Configuration(void)
{
}

bool Configuration::loadCfgFile(std::string filename)
{
    std::ifstream ifs(filename.c_str());

    if(ifs.is_open())
    {
        store(parse_config_file(ifs, description), globalProperties);
        return true;
    }
    else
    {
        throw FileException("Configuration file cannot be opened!");
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

    // parse property to temporary variable map
    variables_map vm;
    store(parse_config_file( stream, description), vm );
    notify(vm);

    // property is already set
    if(globalProperties.count(vm.begin()->first))
    {
        // remove previous value
        globalProperties.erase(vm.begin()->first);
    }

    // add property with new value
    globalProperties.insert( *vm.begin( ) );
}

void Configuration::setProperty(const std::string &grammarID, const std::string &prop)
{
    // was variable map for this grammar already created ?
    if( !grammarNameMap.count( grammarID ) )
    {
        // bind grammar name with index in grammarProperties vector
        grammarNameMap[grammarID] = grammarProperties.size();
        // add variable map for new grammar
        grammarProperties.push_back( variables_map() );
    }

    std::stringstream stream;
    stream << prop;

    // parse property to variable map of current grammar
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
    // index of existing grammar ?
	if(grammarIndex < grammarProperties.size( ))
    {
        // is property set for current grammar ?
		if(grammarProperties[grammarIndex].count(name))
        {
            return &grammarProperties[grammarIndex][name];
        }
    }
    // get property from global properties
    if(globalProperties.count(name))
    {
        return &globalProperties[name];
    }
    else
    {
        return NULL;
    }
}

