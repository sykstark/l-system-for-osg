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

//#include <osgDB/PluginManager.h>
#include <vrecko/ReaderWriter.h>

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Vec3>
#include <osg/ref_ptr>
#include <osg/Image>
#include <osg/Texture2D>
#include <osg/ShapeDrawable>
#include <osg/AlphaFunc>
#include <osg/BlendFunc>
#include <osg/LightModel>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgShadow/SoftShadowMap>
#include <osgShadow/ShadowVolume>
#include <osgShadow/ShadowTexture>
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

//#include "configuration.h"

#endif // _MSC_VER