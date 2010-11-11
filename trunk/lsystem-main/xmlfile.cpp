#include "xmlfile.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <iostream>

#define foreach BOOST_FOREACH

using namespace AP_LSystem;
using boost::property_tree::ptree;

XmlFile::XmlFile()
{
}

void XmlFile::open(std::string & filename)
{
    ptree ls;
    read_xml(filename, ls);

    _name = ls.get<std::string>("lsystem.name");
    axiom = ls.get<std::string>("lsystem.axiom");

    foreach(ptree::value_type & v, ls.get_child("lsystem.constants"))
    {
        defines.insert( std::make_pair<string,string>(v.first.data(),v.second.data()));
    }

    std::cout << defines["high"] << std::endl;
}
