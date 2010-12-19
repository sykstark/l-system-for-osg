#include "precompiled.h"

#include <iostream>
#include <fstream>
#include "configuration.h"
#include "lsystemexception.h"

using namespace AP_LSystem;

// inicialize singleton instance as NULL
Configuration *Configuration::config = 0;

Configuration::Configuration(void)
{
    // description list of all properties
    description.add_options()
            ("Iteration", value<unsigned int>())
            ("ContourDetail", value<unsigned int>())
            ("Color", value<std::string>())
			("DiffuseMaterial", value<std::string>())
			("AmbientMaterial", value<std::string>())
			("SpecularMaterial", value<std::string>())
			("EmissionMaterial", value<std::string>())
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
			("Ignore", value<std::string>())
            ("TurtleType", value<std::string>());
}

Configuration::~Configuration(void)
{
}

const void Configuration::writeAll(std::basic_ostream<char> & out, const string & lsystemID)
{
    out << "Iteration" <<'='<< getProperty( lsystemID, "Iteration" )->as<unsigned>();
}

bool Configuration::loadCfgFile(std::string filename)
{
    std::ifstream ifs(filename.c_str());

    if(ifs.is_open())
    {
		try
		{
			store(parse_config_file(ifs, description), globalProperties);
		}
		catch( std::exception & e )
		{
            ifs.close();
			throw FileException(string("Configuration file parsing error: ") + e.what());
		}
        ifs.close();
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
    // create a singleton
	if(!config) config = new Configuration;

	return config;
}

const int Configuration::getLSystemIndex(const string & name)
{
    // exists L-system with this name ?
    if( lsystemNameMap.count(name))
    {
        // return it's index
        return lsystemNameMap[name];
    }
    else
        return -1;
}

const int Configuration::getLSystemCount()
{
	return lsystemProperties.size();
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

void Configuration::setProperty(const std::string &lsystemID, const std::string &prop)
{
    // was variable map for this lsystem already created ?
    if( !lsystemNameMap.count( lsystemID ) )
    {
        // bind l-system name with index in lsystemProperties vector
        lsystemNameMap[lsystemID] = lsystemProperties.size();
        // add variable map for new lsystem
        lsystemProperties.push_back( variables_map() );
    }

    std::stringstream stream;
    stream << prop;

    // parse property to variable map of current lsystem
    store(parse_config_file( stream, description), lsystemProperties[ lsystemNameMap[lsystemID] ] );
    notify(lsystemProperties[ lsystemNameMap[lsystemID] ]);
}

void Configuration::setProperty(const std::string &lsystemID, const std::string & prop, const std::string & value)
{
    // was variable map for this lsystem already created ?
    if( !lsystemNameMap.count( lsystemID ) )
    {
        // bind lsystem name with index in lsystemProperties vector
        lsystemNameMap[lsystemID] = lsystemProperties.size();
        // add variable map for new lsystem
        lsystemProperties.push_back( variables_map() );
    }

    std::stringstream stream;
    stream << prop << "=" << value;

    // parse property to variable map of current lsystem
	try
	{
		store(parse_config_file( stream, description), lsystemProperties[ lsystemNameMap[lsystemID] ] );
		notify(lsystemProperties[ lsystemNameMap[lsystemID] ]);
	}
	catch( boost::program_options::invalid_option_value & )
	{
		throw ConfigurationException( "Cannot set value of \"" + prop + "\": Invalid value: " + value );
	}
	catch( boost::program_options::unknown_option & e )
	{
		throw ConfigurationException( e.what() );
	}
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

const variable_value * Configuration::getProperty(const std::string &lsystemID, const std::string & name)
{
    if(lsystemNameMap.count(lsystemID))
    {
        if(lsystemProperties[lsystemNameMap[lsystemID]].count(name))
        {
            return &lsystemProperties[lsystemNameMap[lsystemID]][name];
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

const variable_value * Configuration::getProperty(const unsigned int lsystemIndex, const std::string & name)
{
    // index of existing lsystem ?
	if(lsystemIndex < lsystemProperties.size( ))
    {
        // is property set for current lsystem ?
		if(lsystemProperties[lsystemIndex].count(name))
        {
            return &lsystemProperties[lsystemIndex][name];
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

