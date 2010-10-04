#include "precompiled.h"

#include "lsgeode.h"
#include "Configuration.h"

using namespace AP_LSystem;
using namespace std;
using namespace boost::program_options;

void LSGeode::configure()
{
	const string & name = this->getName();

	// configure geode options according to settings in Configuration
	
	Configuration::get()->getProperty( name, "");
}

