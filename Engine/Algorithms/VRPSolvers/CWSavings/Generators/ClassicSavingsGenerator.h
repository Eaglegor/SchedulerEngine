#pragma once

#include "../Savings/ClassicSaving.h"
#include <Engine/Concepts/RoutingProfile.h>
#include <Engine/Routing/RoutingService.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneManager/DepotOperation.h>
#include <Engine/SceneManager/DepotOperation.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkOperation.h>
#include <Engine/SceneManager/WorkOperation.h>
#include <type_traits>

namespace Scheduler
{
	class ClassicSavingsGenerator
	{
	public:
		explicit ClassicSavingsGenerator(const RoutingService& rs)
		    : routing_service(rs)
		{
		}

		template <typename SavingsStore>
		void generateSavings(Scene& scene, SavingsStore& out_store)
		{
			static_assert(std::is_same<typename SavingsStore::value_type, ClassicSaving>::value, "Incompatible savings store");

			Schedule& schedule = scene.getSchedules( )[0];
			RoutingProfile default_routing_profile;

			for(const Order& i : scene.getContext( ).getOrders( ))
			{
				for(const Order& j : scene.getContext( ).getOrders( ))
				{
					Site i_site = i.getWorkOperation( ).getLocation( ).getSite( );
					Site j_site = j.getWorkOperation( ).getLocation( ).getSite( );
					Site d_site = schedule.getPerformer( ).constraints( ).depot( ).get( ).getLocation( ).getSite( );

					Distance id = routing_service.calculateRoute(i_site, d_site, default_routing_profile).getDistance( );
					Distance dj = routing_service.calculateRoute(d_site, j_site, default_routing_profile).getDistance( );
					Distance ij = routing_service.calculateRoute(i_site, j_site, default_routing_profile).getDistance( );

					Distance saving = id + dj - ij;

					out_store.emplace_back(i, j, saving.getValue( ));
				}
			}
		}

	private:
		const RoutingService& routing_service;
	};
}
