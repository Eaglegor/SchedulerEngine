#include "JSONSceneLoader.h"

#include <fstream>
#include "Model/SceneDesc.h"

#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/SceneContext.h>
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

#include <Engine/Concepts/Site.h>
#include <Engine/Utils/Units/DurationUnits.h>

#include <Engine/Algorithms/ScheduleActualization/Route/Default/DefaultRouteActualizationAlgorithm.h>
#include <Engine/Algorithms/ScheduleActualization/Duration/Default/DefaultDurationActualizationAlgorithm.h>
#include <Engine/Algorithms/ScheduleActualization/ArrivalTime/Default/DefaultArrivalTimeActualizationAlgorithm.h>
#include <Engine/Core/Engine.h>

#include <locale>

#include <boost/date_time.hpp>

namespace Scheduler
{
	JSONSceneLoader::JSONSceneLoader(Engine& engine, RoutingService& rs):
		engine(engine),
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

	void parseOperation(const OperationDesc& operation_desc, Operation& out_operation, const LoaderSettings &settings)
	{
		out_operation.setName(operation_desc.name.c_str());

		Capacity load;
		for (size_t i = 0; i < std::min(operation_desc.load.size(), settings.load_dimensions ? settings.load_dimensions.get() : 4); ++i)
		{
			load.setValue(i, operation_desc.load[i]);
		}

		out_operation.constraints().demand().set(load);

		out_operation.constraints().timeWindows().set(createTimeWindows(operation_desc.time_windows, settings));

		if (operation_desc.duration_format)
		{
			out_operation.setDuration(parseDuration(operation_desc.duration_format.get(), operation_desc.duration));
		}
		else
		{
			if (settings.default_duration_format)
			{
				out_operation.setDuration(parseDuration(settings.default_duration_format.get(), operation_desc.duration));
			}
			else
			{
				out_operation.setDuration(parseDuration("", operation_desc.duration));
			}
		}
	}

	Scene& JSONSceneLoader::loadScene(std::istream & stream)
	{
		SceneContext& scene_context = engine.getSceneManager().createSceneContext();

		boost::property_tree::ptree scene_tree;
		boost::property_tree::json_parser::read_json(stream, scene_tree);

		SceneDesc scene_desc = PtreeDeserializer<SceneDesc>()(scene_tree);

		LoaderSettings settings;
		if (scene_desc.settings) settings = scene_desc.settings.get();

		std::unordered_map<std::string, ReferenceWrapper<Location>> locations;
		for (const SiteDesc& location_desc : scene_desc.locations)
		{
			Site location;
			location.setLatitude(Coordinate::createFromFloat(location_desc.latitude));
			location.setLongitude(Coordinate::createFromFloat(location_desc.longitude));
			locations.emplace(location_desc.name, scene_context.createLocation(location));
		}

		std::unordered_map<std::string, ReferenceWrapper<Performer>> performers;
		for (const PerformerDesc &performer_desc: scene_desc.fleet.performers)
		{
			Performer& performer = scene_context.createPerformer();
			performer.setName(performer_desc.name.c_str());
			if (performer_desc.activation_cost) performer.setActivationCost(Cost(performer_desc.activation_cost.get()));
			if (performer_desc.hour_cost) performer.setDurationUnitCost(Cost(performer_desc.hour_cost.get() / Units::hours(1).getValue()));
			performer.constraints().availabilityWindows().set(createTimeWindows(performer_desc.availability_windows, settings));
			
			for (const std::string &skill : performer_desc.skills)
			{
				performer.addSkill(scene_context.createAttribute(skill));
			}

			performers.emplace(performer_desc.name, performer);
		}

		std::unordered_map<std::string, ReferenceWrapper<Vehicle>> vehicles;
		for (const VehicleDesc &vehicle_desc : scene_desc.fleet.vehicles)
		{
			Vehicle& vehicle = scene_context.createVehicle();
			vehicle.setName(vehicle_desc.name);
			if (vehicle_desc.activation_cost) vehicle.setActivationCost(Cost(vehicle_desc.activation_cost.get()));
			if (vehicle_desc.hour_cost) vehicle.setDurationUnitCost(Cost(vehicle_desc.hour_cost.get() / Units::hours(1).getValue()));
			if (vehicle_desc.distance_unit_cost) vehicle.setDistanceUnitCost(Cost(vehicle_desc.distance_unit_cost.get()));

			vehicle.constraints().availabilityWindows().set(createTimeWindows(vehicle_desc.availability_windows, settings));

			for (const std::string &attr : vehicle_desc.attributes)
			{
				vehicle.addAttribute(scene_context.createAttribute(attr));
			}

			RoutingProfile rp;
			rp.setAverageSpeed(Speed(Distance(vehicle_desc.routing_profile.average_speed)));

			vehicle.setRoutingProfile(rp);

			Capacity capacity;
			for (size_t i = 0; i < std::min(vehicle_desc.capacity.size(), settings.load_dimensions ? settings.load_dimensions.get() : 4); ++i)
			{
				capacity.setValue(i, vehicle_desc.capacity[i]);
			}

			vehicle.constraints().capacity().set(capacity);

			vehicles.emplace(vehicle_desc.name, vehicle);
		}

		std::unordered_map<std::string, ReferenceWrapper<Operation>> operations;
		for (const OperationDesc &operation_desc: scene_desc.free_operations)
		{
			Operation& operation = scene_context.createFreeOperation(locations.at(operation_desc.location));
			parseOperation(operation_desc, operation, settings);
			operations.emplace(operation_desc.name, operation);
		}

		for(const OrderDesc &order_desc : scene_desc.orders)
		{
			Order& order = scene_context.createOrder();

			order.setName(order_desc.name);
			
			if(order_desc.start_operation)
			{
				Operation& operation = order.createStartOperation(locations.at(order_desc.start_operation.get().location));
				parseOperation(order_desc.start_operation.get(), operation, settings);
				operation.setName((std::string(order.getName()) + "." + std::string(operation.getName())));
				operations.emplace(operation.getName(), operation);
			}

			for (const OperationDesc &operation_desc : order_desc.work_operations)
			{
				Operation& operation = order.createWorkOperation(locations.at(operation_desc.location));
				parseOperation(operation_desc, operation, settings);
				operation.setName((std::string(order.getName()) + "." + std::string(operation.getName())));
				operations.emplace(operation.getName(), operation);
			}
			
			if (order_desc.end_operation)
			{
				Operation& operation = order.createEndOperation(locations.at(order_desc.end_operation.get().location));
				parseOperation(order_desc.end_operation.get(), operation, settings);
				operation.setName((std::string(order.getName()) + "." + std::string(operation.getName())));
				operations.emplace(operation.getName(), operation);
			}
		}

		Scene& scene = engine.getSceneManager().createScene(scene_context);
		
		for(const ScheduleDesc &schedule_desc : scene_desc.schedules)
		{
			Performer& performer = performers.at(schedule_desc.performer);
			Schedule& schedule = scene.createSchedule(performer);

			schedule.setShift(createTimeWindow(schedule_desc.shift.time_window, settings));

			ScheduleActualizationModel actualization_model;
			DefaultRouteActualizationAlgorithm& route_actualization_algorithm = engine.getAlgorithmsManager().createAlgorithm<DefaultRouteActualizationAlgorithm>(routing_service);
			actualization_model.setRouteActualizationAlgorithm(route_actualization_algorithm);
			DefaultDurationActualizationAlgorithm& duration_actualization_algorithm = engine.getAlgorithmsManager().createAlgorithm<DefaultDurationActualizationAlgorithm>();
			actualization_model.setDurationActualizationAlgorithm(duration_actualization_algorithm);
			DefaultArrivalTimeActualizationAlgorithm& arrival_time_actualization_algorithm = engine.getAlgorithmsManager().createAlgorithm<DefaultArrivalTimeActualizationAlgorithm>();
			actualization_model.setArrivalTimeActualizationAlgorithm(arrival_time_actualization_algorithm);

			schedule.setActualizationModel(actualization_model);

			for(const RunDesc &run_desc : schedule_desc.runs)
			{
				const Location& start_location = locations.at(run_desc.start_location);
				const Location& end_location = locations.at(run_desc.end_location);
				Run& run = *schedule.createRun(schedule.getRuns().end(), start_location, end_location);
				
				Vehicle& vehicle = vehicles.at(run_desc.vehicle);
				run.setVehicle(vehicle);

				for(const StopDesc &stop_desc : run_desc.start_operations)
				{
					run.allocateStartOperation(operations.at(stop_desc.operation));
				}

				for(const StopDesc &stop_desc : run_desc.work_operations)
				{
					Stop& stop = *run.createWorkStop(run.getWorkStops().end(), operations.at(stop_desc.operation));
					stop.setAllocationTime(createTimeWindow(stop_desc.allocation_time, settings));
				}

				for(const StopDesc &stop_desc : run_desc.end_operations)
				{
					run.allocateEndOperation(operations.at(stop_desc.operation));
				}
			}
		}

		return scene;
	}

	Scene& JSONSceneLoader::loadScene(const std::string &filename)
	{
		std::ifstream file;
		file.open(filename.c_str());
		Scene& scene = loadScene(file);
		file.close();
		return scene;
	}
}