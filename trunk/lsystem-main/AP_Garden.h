#ifndef AP_ROOMEDIT
#define AP_ROOMEDIT

#include <vrecko/Ability.h>
#include <vreckoAP/Export>
#include <iostream>

using namespace vrecko;

#pragma warning(disable:4190)
	// disable warning that tells us that using 'extern "C"' and 'string' at the same time is weird

// several functions for maintaining abilities
extern "C" AP_EXPORT const char *Garden_getPluginDescription(); // returns description of this plugin
extern "C" AP_EXPORT char **Garden_getAbilityList(); // returns pointer to the ability name list
extern "C" AP_EXPORT const char *Garden_getAbilityDescription(const char *strName); // returns description of specified ability
extern "C" AP_EXPORT Ability *Garden_getAbilityInstance(const char* pchName); // returns an instance of the specified ability
extern "C" AP_EXPORT void Garden_setAbilitiesParameters(); // sets parameters info for BusinessProcess abilities
extern "C" void AP_EXPORT Garden_initializeAttributesDescription() {}

#endif