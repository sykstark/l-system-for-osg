// Precompiled headers.

// !!!!!!!!!!!
// This file must be included in any other .cpp file in this project
// on the first line (except comments)
// !!!!!!!!!!!

// Documentation on precompiled headers:
// http://www.cygnus-software.com/papers/precompiledheaders.html

#ifdef _MSC_VER

#include <vrecko/Ability.h>
#include <vrecko/EventDispatcher.h>
#include <vrecko/EnvironmentObject.h>
#include <vrecko/Logger.h>
#include <vrecko/Scene.h>

#include <vreckoDB/PluginManager.h>
#include <vreckoDB/ReaderWriter.h>

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Vec3>
#include <osg/ref_ptr>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/ShapeDrawable>
#include <osgDB/ReadFile>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <helpers/XercesXMLUtils.h>

//#include "fparser/fparser.hh"

//#include <../EditorController/ControllableAbility.h>

#include <tchar.h>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <string>
#include <exception>

//#include "Configuration.h"

#define VMATH_NAMESPACE
#include "vmath.h"

#endif // _MSC_VER