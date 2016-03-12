#include "JSONSceneLoader.h"

#include <fstream>
#include "Model/SceneDesc.h"

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Order.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <Engine/Concepts/Basic/Location.h>
#include <Utils/Units/DurationUnits.h>

#include <locale>

namespace Scheduler
{
	JSONSceneLoader::JSONSceneLoader(SceneManager * scene_manager):
		scene_manager(scene_manager)
	{
	}

	Duration parseDuration(const std::string &format, const std::string &duration_string)
	{
		std::tm t;
		std::istringstream ss(duration_string);
		if (!format.empty()) ss >> std::get_time(&t, format.c_str());
		else ss >> std::get_time(&t, "%H:%M:%S");

		std::time_t time = mktime(&t);
		return Duration(time);
	}

	TimePoint parseTimePoint(const std::string &format, const std::string &time_string)
	{
		std::tm t;
		std::istringstream ss(time_string);
		if (!format.empty()) ss >> std::get_time(&t, format.c_str());
		else ss >> std::get_time(&t, "%H:%M:%S");
			
		std::time_t time = mktime(&t);
		return TimePoint(time);
	}

	std::vector<TimeWindow> createTimeWindows(const std::vector<TimeWindowDesc> &desc, const LoaderSettings& settings)
	{
		std::vector<TimeWindow> time_windows;

		for (const TimeWindowDesc &tw_desc : desc)
		{
			TimeWindow time_window;
			
			time_window.setStartTime(parseTimePoint(tw_desc.format ? tw_desc.format.get() : (settings.default_time_format ? settings.default_time_format.get() : ""), tw_desc.start_time));
			time_window.setEndTime(parseTimePoint(tw_desc.format ? tw_desc.format.get() : (settings.default_time_format ? settings.default_time_format.get() : ""), tw_desc.end_time));

			time_windows.push_back(time_window);
		}

		return time_windows;
	}

	void parseOperation(const OperationDesc& operation_desc, Operation* out_operation, const LoaderSettings &settings, const std::unordered_map<std::string, Location> &locations)
	{
		out_operation->setName(operation_desc.name.c_str());
		out_operation->setLocation(locations.at(operation_desc.location));

		Capacity load;
		for (size_t i = 0; i < std::max(operation_desc.load.size(), settings.load_dimensions ? settings.load_dimensions.get() : 4); ++i)
		{
			load.setValue(i, operation_desc.load[i]);
		}

		out_operation->setLoad(load);

		out_operation->setTimeWindows(createTimeWindows(operation_desc.time_windows, settings));

		if (operation_desc.duration_format)
		{
			out_operation->setDuration(parseDuration(operation_desc.duration_format.get(), operation_desc.duration));
		}
		else
		{
			if (settings.default_duration_format)
			{
				out_operation->setDuration(parseDuration(settings.default_duration_format.get(), operation_desc.duration));
			}
			else
			{
				out_operation->setDuration(parseDuration("", operation_desc.duration));
			}
		}
	}

	Scene * JSONSceneLoader::loadScene(std::istream & stream)
	{
		assert(scene_manager);

		Scene* scene = scene_manager->createScene();

		boost::property_tree::ptree scene_tree;
		boost::property_tree::json_parser::read_json(stream, scene_tree);

		SceneDesc scene_desc = PtreeDeserializer<SceneDesc>()(scene_tree);

		LoaderSettings settings;
		if (scene_desc.settings) settings = scene_desc.settings.get();

		std::unordered_map<std::string, Location> locations;
		for (const LocationDesc& location_desc : scene_desc.locations)
		{
			Location location;
			location.setLatitude(Coordinate(location_desc.latitude));
			location.setLongitude(Coordinate(location_desc.longitude));
			locations.emplace(location_desc.name, location);
		}

		std::unordered_map<std::string, Performer*> performers;
		for (const PerformerDesc &performer_desc: scene_desc.fleet.performers)
		{
			Performer* performer = scene->createPerformer();
			performer->setName(performer_desc.name.c_str());
			if (performer_desc.activation_cost) performer->setActivationCost(Cost(performer_desc.activation_cost.get()));
			if (performer_desc.hour_cost) performer->setDurationUnitCost(Cost(performer_desc.hour_cost.get() / Units::hours(1).getValue()));
			performer->setAvailabilityWindows(createTimeWindows(performer_desc.availability_windows, settings));
			
			std::unordered_set<const Attribute*> skills;
			for (const std::string &skill : performer_desc.skills)
			{
				const Attribute* attribute = scene_manager->createAttribute(skill.c_str());
				skills.emplace(attribute);
			}

			performer->setSkills(skills);

			performers.emplace(performer_desc.name, performer);
		}

		std::unordered_map<std::string, Vehicle*> vehicles;
		for (const VehicleDesc &vehicle_desc : scene_desc.fleet.vehicles)
		{
			Vehicle* vehicle = scene->createVehicle();
			vehicle->setName(vehicle_desc.name.c_str());
			if (vehicle_desc.activation_cost) vehicle->setActivationCost(Cost(vehicle_desc.activation_cost.get()));
			if (vehicle_desc.hour_cost) vehicle->setDurationUnitCost(Cost(vehicle_desc.hour_cost.get() / Units::hours(1).getValue()));
			if (vehicle_desc.distance_unit_cost) vehicle->setDistanceUnitCost(Cost(vehicle_desc.distance_unit_cost.get()));

			vehicle->setAvailabilityWindows(createTimeWindows(vehicle_desc.availability_windows, settings));

			std::unordered_set<const Attribute*> attributes;
			for (const std::string &attr : vehicle_desc.attributes)
			{
				const Attribute* attribute = scene_manager->createAttribute(attr.c_str());
				attributes.emplace(attribute);
			}

			vehicle->setAttributes(attributes);

			vehicles.emplace(vehicle_desc.name, vehicle);
		}

		std::unordered_map<std::string, Operation*> free_operations;
		for (const OperationDesc &operation_desc: scene_desc.free_operations)
		{
			Operation* operation = scene->createFreeOperation();
			free_operations.emplace(operation_desc.name, operation);
		}

		std::unordered_map<std::string, Order*> orders;
		for(const OrderDesc &order_desc : scene_desc.orders)
		{
			Order* order = scene->createOrder();

			order->setName(order_desc.name.c_str());
			
			std::unordered_set<const Attribute*> vehicle_requirements;
			for(const std::string &vehicle_requirement : order_desc.vehicle_requirements)
			{
				const Attribute* attribute = scene_manager->createAttribute(vehicle_requirement.c_str());
				vehicle_requirements.emplace(attribute);
			}
			order->setVehicleRequirements(vehicle_requirements);

			std::unordered_set<const Attribute*> performer_skill_requirements;
			for (const std::string &performer_skill_requirement : order_desc.performer_skill_requirements)
			{
				const Attribute* attribute = scene_manager->createAttribute(performer_skill_requirement.c_str());
				performer_skill_requirements.emplace(attribute);
			}
			order->setPerformerSkillsRequirements(performer_skill_requirements);

			if(order_desc.start_operation)
			{
				Operation* operation = order->createStartOperation();
				parseOperation(order_desc.start_operation.get(), operation, settings, locations);
			}

			for (const OperationDesc &operation_desc : order_desc.work_operations)
			{
				Operation* operation = order->createWorkOperation();
				parseOperation(operation_desc, operation, settings, locations);
			}
			
			if (order_desc.end_operation)
			{
				Operation* operation = order->createEndOperation();
				parseOperation(order_desc.end_operation.get(), operation, settings, locations);
			}
		}

		return scene;
	}

	Scene * JSONSceneLoader::loadScene(const std::string &filename)
	{
		std::ifstream file;
		file.open(filename.c_str());
		Scene* scene = loadScene(file);
		file.close();
		return scene;
	}
}