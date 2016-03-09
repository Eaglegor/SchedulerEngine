#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <Persistence/SceneLoaders/JSONSceneLoader/Model/SceneDesc.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

void checkTimeWindow(const boost::property_tree::ptree &tree, const Scheduler::TimeWindowDesc &time_window)
{
	INFO("Checking time window");

	boost::optional<std::string> time_format = tree.get_optional<std::string>("format");
	std::string start_time = tree.get<std::string>("start_time");
	std::string end_time = tree.get<std::string>("end_time");

	REQUIRE(time_format == time_window.format);
	REQUIRE(start_time == time_window.start_time);
	REQUIRE(end_time == time_window.end_time);
	
	INFO("Time window check complete");
}

void checkLocation(const boost::property_tree::ptree &tree, const Scheduler::LocationDesc &location)
{
	INFO("Checking location");

	std::string name = tree.get<std::string>("name");
	float latitude = tree.get<float>("latitude");
	float longitude = tree.get<float>("longitude");

	REQUIRE(name == location.name);
	REQUIRE(latitude == location.latitude);
	REQUIRE(longitude == location.longitude);

	INFO("Location check complete");
}

void checkFleet(const boost::property_tree::ptree &tree, const Scheduler::FleetDesc &fleet)
{
	INFO("Checking fleet");
	INFO("Fleet check complete");
}

void checkLoaderSettings(const boost::property_tree::ptree &tree, const Scheduler::LoaderSettings &settings)
{
	INFO("Checking loader settings");

	boost::optional<size_t> load_dimensions = tree.get_optional<size_t>("load_dimensions");
	boost::optional <std::string> default_time_format = tree.get_optional<std::string>("default_time_format");
	boost::optional <std::string> default_duration_format = tree.get_optional<std::string>("default_duration_format");

	INFO("Loader settings check complete");
}

void checkOperation(const boost::property_tree::ptree &tree, const Scheduler::OperationDesc &operation)
{
	INFO("Checking operation");

	std::string name = tree.get<std::string>("name");
	std::string location = tree.get<std::string>("location");

	const boost::property_tree::ptree &load = tree.get_child("load");
	const boost::property_tree::ptree &time_windows = tree.get_child("time_windows");

	std::string duration = tree.get<std::string>("duration");
	boost::optional<std::string> duration_format = tree.get_optional<std::string>("duration_format");

	REQUIRE(name == operation.name);
	REQUIRE(location == operation.location);

	{
		size_t i = 0;
		for (const auto &iter : load)
		{
			REQUIRE(iter.first.empty());
			REQUIRE(operation.load[i] == iter.second.get_value<float>());
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : time_windows)
		{
			REQUIRE(iter.first.empty());
			checkTimeWindow(iter.second, operation.time_windows[i]);
			++i;
		}
	}

	REQUIRE(duration == operation.duration);
	REQUIRE(duration_format == operation.duration_format);

	INFO("Operation check complete");
}

void checkOrder(const boost::property_tree::ptree &tree, const Scheduler::OrderDesc &order)
{
	INFO("Checking order");

	std::string name = tree.get<std::string>("name");
	const boost::property_tree::ptree &vehicle_requirements = tree.get_child("vehicle_requirements");
	const boost::property_tree::ptree &performer_skill_requirements = tree.get_child("performer_skill_requirements");
	const boost::property_tree::ptree &operations = tree.get_child("operations");
	boost::optional<std::string> start_operation = tree.get_optional<std::string>("start_operation");
	const boost::property_tree::ptree &work_operations = tree.get_child("work_operations");
	boost::optional<std::string>  end_operation = tree.get_optional<std::string>("end_operation");

	REQUIRE(name == order.name);
	
	{
		size_t i = 0;
		for (const auto &iter : vehicle_requirements)
		{
			REQUIRE(iter.first.empty());
			REQUIRE(order.vehicle_requirements[i] == iter.second.get_value<std::string>());
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : performer_skill_requirements)
		{
			REQUIRE(iter.first.empty());
			REQUIRE(order.performer_skill_requirements[i] == iter.second.get_value<std::string>());
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : operations)
		{
			REQUIRE(iter.first.empty());
			checkOperation(iter.second, order.operations[i]);
			++i;
		}
	}

	REQUIRE(start_operation == order.start_operation);

	{
		size_t i = 0;
		for (const auto &iter : work_operations)
		{
			REQUIRE(iter.first.empty());
			REQUIRE(order.work_operations[i] == iter.second.get_value<std::string>());
			++i;
		}
	}

	REQUIRE(end_operation == order.end_operation);

	INFO("Order check complete");
}

void checkPerformer(const boost::property_tree::ptree &tree, const Scheduler::PerformerDesc &performer)
{
	INFO("Checking performer");

	std::string name = tree.get<std::string>("name");

	const boost::property_tree::ptree &availability_windows = tree.get_child("availability_windows");
	const boost::property_tree::ptree &skills = tree.get_child("skills");

	boost::optional<float> hour_cost = tree.get_optional<float>("hour_cost");
	boost::optional<float> activation_cost = tree.get_optional<float>("activation_cost");

	REQUIRE(name == performer.name);

	{
		size_t i = 0;
		for (const auto &iter : availability_windows)
		{
			REQUIRE(iter.first.empty());
			checkTimeWindow(iter.second, performer.availability_windows[i]);
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : skills)
		{
			REQUIRE(iter.first.empty());
			REQUIRE(performer.skills[i] == iter.second.get_value<std::string>());
			++i;
		}
	}

	REQUIRE(hour_cost == performer.hour_cost);
	REQUIRE(activation_cost == performer.activation_cost);

	INFO("Performer check complete");
}

void checkRoutingProfile(const boost::property_tree::ptree &tree, const Scheduler::RoutingProfileDesc &routing_profile)
{
	INFO("Checking routing profile");

	std::string vehicle_type = tree.get<std::string>("vehicle_type");
	float average_speed = tree.get<float>("average_speed");

	REQUIRE(vehicle_type == routing_profile.vehicle_type);
	REQUIRE(average_speed == routing_profile.average_speed);

	INFO("Routing profile check complete");
}

void checkStop(const boost::property_tree::ptree &tree, const Scheduler::StopDesc &stop)
{
	INFO("Checking stop");

	std::string operation = tree.get<std::string>("operation");
	const boost::property_tree::ptree &allocation_time = tree.get_child("allocation_time");
	
	REQUIRE(operation == stop.operation);
	checkTimeWindow(allocation_time, stop.allocation_time);

	INFO("Stop check complete");
}

void checkRun(const boost::property_tree::ptree &tree, const Scheduler::RunDesc &run)
{
	INFO("Checking run");

	std::string vehicle = tree.get<std::string>("vehicle");
	std::string start_location = tree.get<std::string>("start_location");
	std::string end_location = tree.get<std::string>("end_location");

	const boost::property_tree::ptree &stops = tree.get_child("stops");

	REQUIRE(vehicle == run.vehicle);
	REQUIRE(start_location == run.start_location);
	REQUIRE(end_location == run.end_location);

	{
		size_t i = 0;
		for (const auto &iter : stops)
		{
			REQUIRE(iter.first.empty());
			checkStop(iter.second, run.stops[i]);
			++i;
		}
	}

	INFO("Run check complete");
}

void checkScheduleShift(const boost::property_tree::ptree &tree, const Scheduler::ScheduleShiftDesc &schedule_shift)
{
	INFO("Checking schedule shift");

	const boost::property_tree::ptree &time_window = tree.get_child("time_window");
	boost::optional<std::string> start_location = tree.get_optional<std::string>("start_location");
	std::string depot_location = tree.get<std::string>("depot_location");
	boost::optional<std::string> end_location = tree.get_optional<std::string>("end_location");

	checkTimeWindow(time_window, schedule_shift.time_window);
	REQUIRE(start_location == schedule_shift.start_location);
	REQUIRE(depot_location == schedule_shift.depot_location);
	REQUIRE(end_location == schedule_shift.end_location);

	INFO("Schedule shift check complete");
}

void checkVehicle(const boost::property_tree::ptree &tree, const Scheduler::VehicleDesc &vehicle)
{
	INFO("Checking vehicle");

	std::string name = tree.get<std::string>("name");

	const boost::property_tree::ptree &availability_windows = tree.get_child("availability_windows");
	const boost::property_tree::ptree &attributes = tree.get_child("attributes");
	const boost::property_tree::ptree &capacity = tree.get_child("capacity");
	boost::optional<float> hour_cost = tree.get_optional<float>("hour_cost");
	boost::optional<float> distance_unit_cost = tree.get_optional<float>("distance_unit_cost");
	boost::optional<float> activation_cost = tree.get_optional<float>("activation_cost");
	const boost::property_tree::ptree &routing_profile = tree.get_child("routing_profile");

	{
		size_t i = 0;
		for (const auto &iter : availability_windows)
		{
			REQUIRE(iter.first.empty());
			checkTimeWindow(iter.second, vehicle.availability_windows[i]);
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : attributes)
		{
			REQUIRE(iter.first.empty());
			REQUIRE(vehicle.attributes[i] == iter.second.get_value<std::string>());
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : capacity)
		{
			REQUIRE(iter.first.empty());
			REQUIRE(vehicle.capacity[i] == iter.second.get_value<float>());
			++i;
		}
	}

	REQUIRE(hour_cost == vehicle.hour_cost);
	REQUIRE(distance_unit_cost == vehicle.distance_unit_cost);
	REQUIRE(activation_cost == vehicle.activation_cost);

	checkRoutingProfile(routing_profile, vehicle.routing_profile);

	INFO("Vehicle check complete");
}

void checkSchedule(const boost::property_tree::ptree &tree, const Scheduler::ScheduleDesc &schedule)
{
	INFO("Checking schedule");

	std::string performer = tree.get<std::string>("performer");
	const boost::property_tree::ptree &shift = tree.get_child("shift");
	const boost::property_tree::ptree &runs = tree.get_child("runs");

	REQUIRE(performer == schedule.performer);
	checkScheduleShift(shift, schedule.shift);
	
	{
		size_t i = 0;
		for (const auto &iter : runs)
		{
			REQUIRE(iter.first.empty());
			checkRun(iter.second, schedule.runs[i]);
			++i;
		}
	}

	INFO("Schedule check complete");
}

void checkScene(const boost::property_tree::ptree &tree, const Scheduler::SceneDesc &scene)
{
	INFO("Checking scene");

	boost::optional<const boost::property_tree::ptree &> settings = tree.get_child_optional("settings");
	const boost::property_tree::ptree &locations = tree.get_child("locations");
	const boost::property_tree::ptree &orders = tree.get_child("orders");
	const boost::property_tree::ptree &free_operations = tree.get_child("free_operations");
	const boost::property_tree::ptree &fleet = tree.get_child("fleet");
	const boost::property_tree::ptree &schedules = tree.get_child("schedules");

	if(settings)
	{
		checkLoaderSettings(settings.get(), scene.settings.get());
	}

	{
		size_t i = 0;
		for (const auto &iter : locations)
		{
			REQUIRE(iter.first.empty());
			checkLocation(iter.second, scene.locations[i]);
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : orders)
		{
			REQUIRE(iter.first.empty());
			checkOrder(iter.second, scene.orders[i]);
			++i;
		}
	}

	{
		size_t i = 0;
		for (const auto &iter : free_operations)
		{
			REQUIRE(iter.first.empty());
			checkOperation(iter.second, scene.free_operations[i]);
			++i;
		}
	}

	checkFleet(fleet, scene.fleet);

	{
		size_t i = 0;
		for (const auto &iter : schedules)
		{
			REQUIRE(iter.first.empty());
			checkSchedule(iter.second, scene.schedules[i]);
			++i;
		}
	}

	INFO("Scene check complete");
}

TEST_CASE("Persistence - SceneLoaders - JSONSceneLoader - ModelLoading", "[unit][functional][persistence]")
{
	boost::property_tree::ptree props;

	std::string test_json = "{\
  \"settings\": {\
    \"load_dimensions\": 1,\
    \"default_time_format\": \"hh:mm\",\
    \"default_duration_format\": \"hh:mm\"\
  },\
  \"locations\": [\
    {\
      \"name\": \"DC_Location\",\
      \"latitude\": 34.567,\
      \"longitude\": 23.567\
    },\
    {\
      \"name\": \"Order1_Location\",\
      \"latitude\": 23.65,\
      \"longitude\": 23.64\
    },\
    {\
      \"name\": \"Order2_Pickup\",\
      \"latitude\": 25.454,\
      \"longitude\": 23.5353\
    },\
    {\
      \"name\": \"Order2_Drop\",\
      \"latitude\": 43.64,\
      \"longitude\": 54.75\
    },\
    {\
      \"name\": \"Order3_Location\",\
      \"latitude\": 54.23,\
      \"longitude\": 23.64\
    },\
    {\
      \"name\": \"FreeOperation1_Location\",\
      \"latitude\": 23.45,\
      \"longitude\": 23.53\
    }\
  ],\
  \"orders\": [\
    {\
      \"name\": \"Order1\",\
      \"vehicle_requirements\": [\
        \"Attr1\",\
        \"Attr2\"\
      ],\
      \"performer_skill_requirements\": [],\
      \"operations\": [\
        {\
          \"name\": \"pickup\",\
          \"location\": \"DC_Location\",\
          \"load\": [\
            1\
          ],\
          \"duration\": \"00:03\",\
          \"time_windows\": [\
            {\
              \"start_time\": \"08:00\",\
              \"end_time\": \"18:00\"\
            }\
          ]\
        },\
        {\
          \"name\": \"drop\",\
          \"location\": \"Order1_Location\",\
          \"load\": [\
            -1\
          ],\
          \"time_windows\": [\
            {\
              \"start_time\": \"10:00\",\
              \"end_time\": \"10:30\"\
            }\
          ],\
          \"duration\": \"00:10\"\
        }\
      ],\
      \"start_operation\": \"pickup\",\
      \"work_operations\": [\
        \"drop\"\
      ],\
      \"end_operation\": null\
    },\
    {\
      \"name\": \"Order2\",\
      \"vehicle_requirements\": [],\
      \"performer_skill_requirements\": [],\
      \"operations\": [\
        {\
          \"name\": \"collection\",\
          \"location\": \"Order2_Pickup\",\
          \"load\": [\
            2\
          ],\
          \"time_windows\": [\
            {\
              \"start_time\": \"10:00\",\
              \"end_time\": \"12:00\"\
            }\
          ],\
          \"duration\": \"00:10\"\
        },\
        {\
          \"name\": \"delivery\",\
          \"location\": \"Order2_Drop\",\
          \"load\": [\
            -2\
          ],\
          \"time_windows\": [\
            {\
              \"start_time\": \"12:00\",\
              \"end_time\": \"15:00\"\
            }\
          ],\
          \"duration\": \"00:10\"\
        }\
      ],\
      \"start_operation\": null,\
      \"work_operations\": [\
        \"collection\",\
        \"delivery\"\
      ],\
      \"end_operation\": null\
    },\
    {\
      \"name\": \"Order3\",\
      \"vehicle_requirements\": [],\
      \"performer_skill_requirements\": [],\
      \"operations\": [\
        {\
          \"name\": \"work\",\
          \"location\": \"Order2_Pickup\",\
          \"load\": [\
            0\
          ],\
          \"time_windows\": [\
            {\
              \"start_time\": \"12:00\",\
              \"end_time\": \"16:00\"\
            }\
          ],\
          \"duration\": \"00:10\"\
        }\
      ],\
      \"start_operation\": null,\
      \"work_operations\": [\
        \"pickup\",\
        \"FreeOperation1\"\
      ]\
    }\
  ],\
  \"free_operations\": [\
    {\
      \"name\": \"FreeOperation1\",\
      \"location\": \"FreeOperation1_Location\",\
      \"load\": [\
        0\
      ],\
      \"time_windows\": [\
        {\
          \"start_time\": \"12:00\",\
          \"end_time\": \"18:00\"\
        }\
      ],\
      \"duration\": \"00:10\"\
    }\
  ],\
  \"fleet\": {\
    \"performers\": [\
      {\
        \"name\": \"Driver1\",\
        \"availability_windows\": [\
          {\
            \"start_time\": \"08:00\",\
            \"end_time\": \"18:00\"\
          }\
        ],\
        \"skills\": [\
          \"Driving\"\
        ],\
        \"hour_cost\": 0,\
        \"activation_cost\": 0\
      }\
    ],\
    \"vehicles\": [\
      {\
        \"name\": \"Vehicle1\",\
        \"capacity\": [\
          3\
        ],\
        \"availability_windows\": [\
          {\
            \"start_time\": \"08:00\",\
            \"end_time\": \"18:00\"\
          }\
        ],\
        \"attributes\": [\
          \"Attr1\",\
          \"Attr2\"\
        ],\
        \"routing_profile\": {\
          \"vehicle_type\" : \"Truck\",\
          \"average_speed\": 60\
        },\
        \"hour_cost\": 0,\
        \"distance_unit_cost\": 1,\
        \"activation_cost\": 0\
      }\
    ]\
  },\
  \"schedules\": [\
    {\
      \"performer\": \"Driver1\",\
      \"shift\": {\
        \"time_window\": {\
          \"start_time\": \"08:00\",\
          \"end_time\": \"18:00\"\
        },\
        \"start_location\": null,\
        \"depot_location\": \"DC_Location\",\
        \"end_location\": null\
      },\
      \"runs\": [\
        {\
          \"vehicle\": \"Vehicle1\",\
          \"start_location\": \"DC_Location\",\
          \"end_location\": \"DC_Location\",\
          \"stops\": [\
            {\
              \"operation\": \"Order1.start\",\
              \"allocation_time\": {\
                \"start_time\": \"08:00\",\
                \"end_time\": \"08:03\"\
              }\
            },\
            {\
              \"operation\": \"Order1.work[0]\",\
              \"allocation_time\": {\
                \"start_time\": \"09:00\",\
                \"end_time\": \"09:10\"\
              }\
            }\
          ]\
        }\
      ]\
    }\
  ]\
}";

	std::istringstream stream(test_json);

	read_json(stream, props);

	Scheduler::SceneDesc desc = Scheduler::PtreeDeserializer<Scheduler::SceneDesc>()(props);
	
	checkScene(props, desc);
}