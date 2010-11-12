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
            ("Iteration", value<unsigned int>())
            ("ContourDetail", value<unsigned int>())
            ("DiffuseTexture", value<std::string>())
            ("TextureSRepeatings", value<unsigned int>())
            ("GravitropismElasticity", value<double>())
            ("TropismElasticity", value<double>())
            ("TropismAngle", value<double>())
            ("TropismVector", value<std::string>())
            ("DefaultAngle", value<double>())
            ("DefaultLength", value<double>())
            ("DefaultRadius", value<double>())
            ("AngleMultiplier", value<double>())
            ("LengthMultiplier", value<double>())
            ("RadiusMultiplier", value<double>())
            ("DebugGeometryScale", value<double>())
            ("MinimizeTwist", value<unsigned int>())
            ("DegreesToRadians", value<unsigned int>())
            ("DrawDebugGeometry", value<unsigned int>())
            ("DrawPipeCaps", value<unsigned int>())
            ("AngleVariance", value<unsigned int>())
            ("UseQueries", value<unsigned int>())
            ("SeparateGeometryForTranslucent", value<unsigned int>())
            ("TurtleType", value<std::string>());
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

