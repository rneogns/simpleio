#include "Precompiled.h"
#include "ConfigReader.h"
#include "StringUtil.h"
#include "Log.h"
#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;

bool ConfigReader::Read(string filename)
{
	try
	{
		boost::property_tree::read_ini(filename, pt);
		return true;
	}
	catch (std::exception& e)
	{
		LOG_CORE("Fail to read %s config file (%s)", filename.c_str(), e.what());
		return false;
	}
}