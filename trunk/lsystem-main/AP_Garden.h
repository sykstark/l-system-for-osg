#ifndef AP_GARDEN_H_
#define AP_GARDEN_H_

#include <vrecko/Ability.h>
#include <vreckoAP/Export>
#include <iostream>

using namespace vrecko;

/**
 * @mainpage L-system plant modeler - module for Vrecko
 * @section About
 * This DLL library was developed as part of my Master thesis. It is a plant modeler plug-in for virtual reality 
 * system Vrecko. It is based on OpenSceneGraph. There is a parser for L-systems stored in LS or XML files. Many types of L-systems can be processed
 * to generate a word. Word is then interpreted by turtle graphics. Interpretation process generates a geometry that
 * is stored in scene graph. Vrecko then display this geometry.
 * @section Features
 * <ul>
 *    <li> generating plants using L-systems </li>
 *    <li> D0L-systems, parametric stochastic 0Lsystems, parametric stochastic 2L-systems </li>
 *    <li> generating plants geometry using Turtle interpretation </li>
 * </ul>
 * @section Links
 * <ul>
 *    <li> Vrecko: http://decibel.fi.muni.cz/wiki/index.php/Main_Page </li>
 *    <li> Project website and svn: http://code.google.com/p/l-system-for-osg </li>
 * </ul>
 */

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
