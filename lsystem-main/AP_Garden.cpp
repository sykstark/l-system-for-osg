#include "precompiled.h"

#include "AP_Garden.h"
#include "lsobject.h"

using namespace AP_LSystem;

// plug-in and ability names, ability descriptions
char *g_pAPGardenPluginName = {"Garden"};


char *g_pAPGardenAbilityName[] = {
	"LSObject",
	NULL
};

char *g_pAPGardenAbilityDesc[] = {
	"LSObject description",
	""
};

extern "C" AP_EXPORT const char *Garden_getPluginDescription()
{
	return "Garden";
}

extern "C" AP_EXPORT char **Garden_getAbilityList()
{
	return g_pAPGardenAbilityName;
}

extern "C" AP_EXPORT const char *Garden_getAbilityDescription(const char *strName)
{
	// prepare the ability index
	int nIndex = 0;

	// find index of the ability
	while(g_pAPGardenAbilityName[nIndex] && strcmp(strName,g_pAPGardenAbilityName[nIndex]) != 0) nIndex++;
	return g_pAPGardenAbilityDesc[nIndex];
}

extern "C" AP_EXPORT Ability *Garden_getAbilityInstance(const char* pchName)
{
	// prepare Ability pointer and set it to NULL
	Ability* pAbility = NULL;

	// create desired instance of the ability
	if (!strcmp (pchName, "LSObject")) {
		pAbility = new LSObject();
	}

	// if successfully created
	if (pAbility != NULL)
	{
		// set the plug-in name
		pAbility->setPluginName(g_pAPGardenPluginName);
	}

	return pAbility;
}

extern "C" AP_EXPORT void Garden_setAbilitiesParameters()
{
	// not needed now, will be used in future implementations of editor

	/*// prepare temp variables
	Ability::ParameterVector* pParameterVector;
	Ability::ParameterStruct parameterStruct;

	// create new parameter vector on the heap
	pParameterVector = new Ability::ParameterVector;

	// add transparency of active form parameter info
	parameterStruct.name = "ActFrmTrp";
	parameterStruct.description = "Transparency of active form";
	parameterStruct.type = BaseClass::FLOAT;
	parameterStruct.range = "<0.0,1.0>";
	pParameterVector->insert(pParameterVector->end(), parameterStruct);

	// add transparency of active form element parameter info
	parameterStruct.name = "ActFrmElemTrp";
	parameterStruct.description = "Transparency of active form element";
	parameterStruct.type = BaseClass::VEC3;
	parameterStruct.range = "<0.0,1.0>";
	pParameterVector->insert(pParameterVector->end(), parameterStruct);

	// set parameter vector for the BusinessProcess ability
	BusinessProcess::m_pParameterVector = pParameterVector;*/
}
