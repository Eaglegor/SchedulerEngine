#include "JSONSceneLoader.h"

#include <fstream>
#include "Model/SceneDesc.h"

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Vehicle.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/WorkStop.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <Engine/Concepts/Location.h>
#include <Engine/Utils/Units/DurationUnits.h>

#include <Engine/Algorithms/ScheduleActualization/Route/Default/DefaultRouteActualizationAlgorithm.h>
#include <Engine/Algorithms/ScheduleActualization/Duration/Default/DefaultDurationActualizationAlgorithm.h>
#include <Engine/Algorithms/ScheduleActualization/ArrivalTime/Default/DefaultArrivalTimeActualizationAlgorithm.h>

#include <locale>

#include <boost/date_time.hpp>

namespace Scheduler
{
	JSONSceneLoader::JSONSceneLoader(SceneManager * scene_manager, RoutingService* rs):
		scene_manager(scene_manager),
		routing_service(rs)
	{
	}

	Duration parseDuration(const std::string &format, const std::string &duration_string)
	{
		boost::posix_time::ptime time;

		// This must be a pointer because std::local will try to delete it on destruction
		boost::posix_time::time_input_facet *facet = new boost::posix_time::time_input_facet{ format.empty() ? format : "%H:%M:%S" };

		std::istringstream ss(duration_string);
		ss.imbue(std::locale(std::locale(), facet));
		ss >> time;

		boost::posix_time::time_duration dur = time - boost::posix_time::ptime(boost::gregorian::date(1400, 1, 1));

		return Duration(dur.total_seconds());
	}

	TimePoint parseTimePoint(const std::string &format, const std::string &time_string)
	{
		boost::posix_time::ptime time;

		// This must be a pointer because std::local will try to delete it on destruction
		boost::posix_time::time_input_facet *facet = new boost::posix_time::time_input_facet{ format.empty() ? format : "%H:%M:%S" };

		std::istringstream ss(time_string);
		ss.imbue(std::locale(std::locale(), facet));
		ss >> time;
		
		boost::posix_time::time_duration dur = time - boost::posix_time::ptime(boost::gregorian::date(1400, 1, 1));

		return TimePoint(dur.total_seconds());
	}

	TimeWindow createTimeWindow(const TimeWindowDesc &desc, const LoaderSettings& settings)
	{
		TimeWindow time_window;

		time_window.setStartTime(parseTimePoint(desc.format ? desc.format.get() : (settings.default_time_format ? settings.default_time_format.get() : ""), desc.start_time));
		time_window.setEndTime(parseTimePoint(desc.format ? desc.format.get() : (settings.default_time_format ? settings.default_time_format.get() : ""), desc.end_time));

		return time_window;
	}

	std::vector<TimeWindow> createTimeWindows(const std::vector<TimeWindowDesc> &desc, const LoaderSettings& settings)
	{
		std::vector<TimeWindow> time_windows;

		for (const TimeWindowDesc &tw_desc : desc)
		{
			time_windows.push_back(createTimeWindow(tw_desc, settings));
		}

		return time_windows;
	}

	void parseOperation(const OperationDesc& operation_desc, Operation* out_operation, const LoaderSettings &settings, const std::unordered_map<std::string, Location> &locations)
	{
		out_operation->setName(operation_desc.name.c_str());
		out_operation->setLocation(locations.at(operation_desc.location));

		Capacity load;
		for (size_t i = 0; i < std::min(operation_desc.load.size(), settings.load_dimensions ? settings.load_dimensions.get() : 4); ++i)
		{
			load.setValue(i, operation_desc.load[i]);
		}

		out_operation->setDemand(load);

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

			RoutingProfile rp;
			rp.setAverageSpeed(Speed(Distance(vehicle_desc.routing_profile.average_speed)));

			vehicle->setRoutingProfile(rp);

			Capacity capacity;
			for (size_t i = 0; i < std::min(vehicle_desc.capacity.size(), settings.load_dimensions ? settings.load_dimensions.get() : 4); ++i)
			{
				capacity.setValue(i, vehicle_desc.capacity[i]);
			}

			vehicle->setCapacity(capacity);

			vehicles.emplace(vehicle_desc.name, vehicle);
		}

		std::unordered_map<std::string, Operation*> operations;
		for (const OperationDesc &operation_desc: scene_desc.free_operations)
		{
			Operation* operation = scene->createFreeOperation();
			parseOperation(operation_desc, operation, settings, locations);
			operations.emplace(operation_desc.name, operation);
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
				operation->setName((std::string(order->getName()) + "." + std::string(operation->getName())).c_str());
				operations.emplace(operation->getName(), operation);
			}

			for (const OperationDesc &operation_desc : order_desc.work_operations)
			{
				Operation* operation = order->createWorkOperation();
				parseOperation(operation_desc, operation, settings, locations);
				operation->setName((std::string(order->getName()) + "." + std::string(operation->getName())).c_str());
				operations.emplace(operation->getName(), operation);
			}
			
			if (order_desc.end_operation)
			{
				Operation* operation = order->createEndOperation();
				parseOperation(order_desc.end_operation.get(), operation, settings, locations);
				operation->setName((std::string(order->getName()) + "." + std::string(operation->getName())).c_str());
				operations.emplace(operation->getName(), operation);
			}
		}

		for(const ScheduleDesc &schedule_desc : scene_desc.schedules)
		{
			Performer* performer = performers.at(schedule_desc.performer);
			Schedule* schedule = scene->createSchedule(performer);
			if (schedule_desc.shift.start_location) schedule->setShiftStartLocation(locations.at(schedule_desc.shift.start_location.get()));
			schedule->setDepotLocation(locations.at(schedule_desc.shift.depot_location));
			if (schedule_desc.shift.end_location) schedule->setShiftEndLocation(locations.at(schedule_desc.shift.end_location.get()));

			schedule->setShift(createTimeWindow(schedule_desc.shift.time_window, settings));

			ScheduleActualizationModel* actualization_model = scene_manager->createScheduleActualizationModel();
			DefaultRouteActualizationAlgorithm* route_actualization_algorithm = scene_manager->createRouteActualizationAlgorithm<DefaultRouteActualizationAlgorithm>(routing_service);
			actualization_model->setRouteActualizationAlgorithm(route_actualization_algorithm);
			DefaultDurationActualizationAlgorithm* duration_actualization_algorithm = scene_manager->createDurationActualizationAlgorithm<DefaultDurationActualizationAlgorithm>();
			actualization_model->setDurationActualizationAlgorithm(duration_actualization_algorithm);
			DefaultArrivalTimeActualizationAlgorithm* arrival_time_actualization_algorithm = scene_manager->createArrivalTimeActualizationAlgorithm<DefaultArrivalTimeActualizationAlgorithm>();
			actualization_model->setArrivalTimeActualizationAlgorithm(arrival_time_actualization_algorithm);

			schedule->setActualizationModel(actualization_model);

			for(const RunDesc &run_desc : schedule_desc.runs)
			{
				Location start_location = locations.at(run_desc.start_location);
				Location end_location = locations.at(run_desc.end_location);
				Run* run = schedule->createRun(start_location, end_location);
				
				Vehicle* vehicle = vehicles.at(run_desc.vehicle);
				run->setVehicle(vehicle);

				for(const StopDesc &stop_desc : run_desc.start_operations)
				{
					run->allocateStartOperation(operations.at(stop_desc.operation));
				}

				for(const StopDesc &stop_desc : run_desc.work_operations)
				{
					Stop* stop = run->allocateWorkOperation(operations.at(stop_desc.operation));
					stop->setAllocationTime(createTimeWindow(stop_desc.allocation_time, settings));
				}

				for(const StopDesc &stop_desc : run_desc.end_operations)
				{
					run->allocateEndOperation(operations.at(stop_desc.operation));
				}
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