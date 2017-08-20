#pragma once

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <type_traits>
#include <vector>

namespace Scheduler
{
	template <typename T>
	struct PtreeDeserializer
	{
		T operator( )(const boost::property_tree::ptree& tree)
		{
			return tree.get_value<T>( );
		}
	};

	template <>
	struct PtreeDeserializer<boost::property_tree::ptree>
	{
		boost::property_tree::ptree operator( )(const boost::property_tree::ptree& tree)
		{
			return tree;
		}
	};

	template <typename U>
	struct PtreeDeserializer<boost::optional<U>>
	{
		U operator( )(const boost::property_tree::ptree& tree)
		{
			return PtreeDeserializer<U>( )(tree);
		}
	};

	template <typename U>
	struct PtreeDeserializer<std::vector<U>>
	{
		std::vector<U> operator( )(const boost::property_tree::ptree& tree)
		{
			std::vector<U> result;

			for(const auto& iter : tree)
			{
				assert(iter.first.empty( ));
				result.push_back(PtreeDeserializer<U>( )(iter.second));
			}

			return result;
		}
	};

#define REQUIRED_SET(tree, descriptor, field_name) descriptor.field_name = PtreeDeserializer<decltype(descriptor.field_name)>( )(tree.get_child(#field_name));
#define OPTIONAL_SET(tree, descriptor, field_name) \
	if(tree.get_child_optional(#field_name) && tree.get<std::string>(#field_name) != "null") descriptor.field_name = PtreeDeserializer<decltype(descriptor.field_name)>( )(tree.get_child(#field_name));
#define OPTIONAL_SET_OR_DEFAULT(tree, descriptor, field_name, default_value)                                        \
	if(tree.get_child_optional(#field_name) && tree.get<std::string>(#field_name) != "null")                        \
		descriptor.field_name = PtreeDeserializer<decltype(descriptor.field_name)>( )(tree.get_child(#field_name)); \
	else                                                                                                            \
		descriptor.field_name = default_value;
}