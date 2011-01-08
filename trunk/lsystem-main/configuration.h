#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <sstream>
#include <map>
#include <boost/program_options.hpp>

using namespace boost::program_options;
using std::vector;
using std::string;

namespace AP_LSystem {

/**
  * All L-system capabilities. L-system type is built as bitmap from this values.
  */
enum LSystemCapabilities
{
    LS_0L               = 0x00000001,   ///< 0L-system
    LS_1LL              = 0x00000002,   ///< 1L-system with left context
    LS_1LR              = 0x00000004,   ///< 1L-system with right context
    LS_2L               = 0x00000008,   ///< 2L-system with left and right context
    LS_IL               = 0x00000080,   ///< IL-system
    LS_DETERMINISTIC    = 0x00000100,   ///< Deterministic L-system
    LS_STOCHASTIC       = 0x00000200,   ///< Stochastic L-system
    LS_PARAMETRIC       = 0x00010000,   ///< Parametric L-system
};

/**
  * Configuration class for storing all properties of all L-systems. Every property must be set
  * globally. It is possible to set properties also only for particular L-systems. This value
  * has higher priority.
  */
class Configuration
{
private:
    static Configuration* config;                   ///< singleton instance

    options_description description;                ///< description of properties
    variables_map globalProperties;                 ///< map for storing global properties values
    std::vector<variables_map> lsystemProperties;   ///< map for storing properties values of particular L-systems
    std::map<std::string, int> lsystemNameMap;      ///< map of L-system indexes

    /**
      * Private constructor - this class is singleton
      */
    Configuration();

    /**
      * Privated destructor - this class is singleton
      */
    ~Configuration();
public:
    /**
      * Loads a configuration file. Loaded properties are marked as global
      * @param filename configuration file filename
      * @return true if succeeded
      */
    bool loadCfgFile(string filename);

    /**
      * Sets global property value
      * @param prop property string in following form: (property)=(value)
      */
    void setProperty(const string & prop);

    /**
      * Sets property only for one particular L-system
      * @param lsystemID name of L-system
      * @param prop property string in following form: (property)=(value)
      */
    void setProperty(const string & lsystemID, const string & prop);

    /**
      * Sets property only for one particular L-system
      * @param lsystemID name of L-system
      * @param prop property name
      * @param value property value
      */
    void setProperty(const string & lsystemID, const string & prop, const string & value);

    /**
      * Returns unique integer identifier of L-system
      * @param lsystemID name of L-system
      * @return unique index of L-system
      */
    const int getLSystemIndex( const string & lsystemID );

    /**
      * Returns number L-systems that have stored some properties
      * @return number of L-systems
      */
	const int getLSystemCount();

    /**
      * Returns property global value
      * @param prop property name
      * @return property value
      */
    const variable_value * getProperty(const string & prop);

    /**
      * Returns property value of one particular L-system
      * @param lsystemID name of L-system
      * @param prop property name
      * @return property value
      */
    const variable_value * getProperty(const string & lsystemID, const string & prop);


    /**
      * Returns property value of one particular L-system
      * @param lsystemIndex index of L-system
      * @param prop property name
      * @return property value
      */
    const variable_value * getProperty(const unsigned int lsystemIndex, const string & prop);

    /**
      * Returns instance of Configuration singleton
      * @return Configuration instance
      */
	static Configuration * get();

    /**
      * Not implemented
      */
    const void writeAll( std::basic_ostream<char> &, const string & );
};
}

#endif
