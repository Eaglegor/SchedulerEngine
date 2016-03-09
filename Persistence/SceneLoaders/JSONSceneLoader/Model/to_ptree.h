#pragma once

#include <boost/property_tree/ptree.hpp>

namespace Scheduler
{
	template<typename T>
	boost::property_tree::ptree to_ptree(const T &obj)
	{
		return boost::property_tree::ptree();
	}

	template<>
	boost::property_tree::ptree to_ptree<int>(const int &obj)
	{
		boost::property_tree::ptree tree;
		tree.put_value(obj);
		return tree;
	}

	template<>
	boost::property_tree::ptree to_ptree<long>(const long &obj)
	{
		boost::property_tree::ptree tree;
		tree.put_value(obj);
		return tree;
	}

	template<>
	boost::property_tree::ptree to_ptree<float>(const float &obj)
	{
		boost::property_tree::ptree tree;
		tree.put_value(obj);
		return tree;
	}

	template<>
	boost::property_tree::ptree to_ptree<double>(const double &obj)
	{
		boost::property_tree::ptree tree;
		tree.put_value(obj);
		return tree;
	}

	template<>
	boost::property_tree::ptree to_ptree<std::string>(const std::string &obj)
	{
		boost::property_tree::ptree tree;
		tree.put_value(obj);
		return tree;
	}

	template<>
	boost::property_tree::ptree to_ptree<bool>(const bool &obj)
	{
		boost::property_tree::ptree tree;
		tree.put_value(obj);
		return tree;
	}

}