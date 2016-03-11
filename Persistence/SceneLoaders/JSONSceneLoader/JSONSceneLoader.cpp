#include "JSONSceneLoader.h"

#include <fstream>
#include "Model/SceneDesc.h"

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <Engine/Concepts/Basic/Location.h>
#include <Utils/Units/DurationUnits.h>

namespace Scheduler
{
	JSONSceneLoader::JSONSceneLoader(SceneManager * scene_manager):
		scene_manager(scene_manager)
	{
	}

	Duration parseDuration(const std::string &format, const std::string &duration_string)
	{
		return Duration();
	}

	TimePoint parseTimePoint(const std::string &format, const std::string &time_string)
	{
		return TimePoint();
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

	Scene * JSONSceneLoader::loadScene(std::istream & stream)
	{
		assert(scene_manager);

		Scene* scene = scene_manager->createScene();

		boost::property_tree::ptree scene_tree;
		boost::property_tree::json_parser::read_json(stream, scene_tree);

		SceneDesc scene_desc = PtreeDeserializer<SceneDesc>()(scene_tree);

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
			performer->setAvailabilityWindows(createTimeWindows(performer_desc.availability_windows, scene_desc.settings ? scene_desc.settings.get() : LoaderSettings()));
			
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

			vehicle->setAvailabilityWindows(createTimeWindows(vehicle_desc.availability_windows, scene_desc.settings ? scene_desc.settings.get() : LoaderSettings()));

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

			operation->setName(operation_desc.name.c_str());
			operation->setLocation(locations[operation_desc.location]);
			
			Capacity load;
			for (size_t i = 0; i < std::max(operation_desc.load.size(), scene_desc.settings ? (scene_desc.settings.get().load_dimensions ? scene_desc.settings.get().load_dimensions.get() : 4) : 4); ++i)
			{
				load.setValue(i, operation_desc.load[i]);
			}
			operation->setLoad(load);

			operation->setTimeWindows(createTimeWindows(operation_desc.time_windows, scene_desc.settings ? scene_desc.settings.get() : LoaderSettings()));

			if (operation_desc.duration_format)
			{
				operation->setDuration(parseDuration(operation_desc.duration_format.get(), operation_desc.duration));
			}
			else
			{
				if (scene_desc.settings)
				{
					if (scene_desc.settings.get().default_duration_format)
					{
						operation->setDuration(parseDuration(scene_desc.settings.get().default_duration_format.get(), operation_desc.duration));
					}
					else
					{
						operation->setDuration(parseDuration("", operation_desc.duration));
					}
				}
				else
				{
					operation->setDuration(parseDuration("", operation_desc.duration));
				}
			}
			
			free_operations.emplace(operation_desc.name, operation);
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