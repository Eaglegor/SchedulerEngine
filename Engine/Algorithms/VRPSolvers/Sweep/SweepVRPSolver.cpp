#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include "SweepVRPSolver.h"
#include <Engine/SceneEditor/Actions/AllocateOrder.h>
#include <Engine/SceneEditor/Actions/CreateRun.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneManager/DepotOperation.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/WorkOperation.h>
#include <algorithm>
#include <vector>

namespace Scheduler
{
	SweepVRPSolver::SweepVRPSolver( )
	{
	}
	SweepVRPSolver::~SweepVRPSolver( )
	{
	}

	void SweepVRPSolver::optimize(Scene& scene) const
	{
		struct ByAngle
		{
			explicit ByAngle(const Site& center)
			    : center_location(center)
			{
			}
			bool operator( )(const Order& lhs, const Order& rhs) const
			{
				return angle(lhs) < angle(rhs);
			}
			float angle(const Order& order) const
			{
				return angle(order.getWorkOperation( ).getLocation( ).getSite( ));
			}
			float angle(const Site& location) const
			{
				const float x     = (location.getLatitude( ) - center_location.getLatitude( )).getValue( );
				const float y     = (location.getLongitude( ) - center_location.getLongitude( )).getValue( );
				const float hypot = std::hypot(x, y);
				const float angle = std::acos(y / hypot);
				return x > 0.f ? angle : 2 * M_PI - angle;
			}
			float degreeAngle(const Site& location) const
			{
				return angle(location) * 180.f / M_PI;
			}

			Site center_location;
		};

		auto& schedules = scene.getSchedules( );
		auto& orders    = scene.getContext( ).getOrders( );

		std::vector<ReferenceWrapper<const Order>> sorted_orders;

		for(const Order& order : orders)
		{
			sorted_orders.emplace_back(order);
		}

		if(sorted_orders.empty( ))
		{
			return;
		}

		SceneEditor scene_editor;

		for(Schedule& schedule : schedules)
		{
			const Location& depot_location = schedule.getPerformer( ).constraints( ).depot( ).get( ).getLocation( );
			ByAngle by_angle(depot_location.getSite( ));
			std::sort(sorted_orders.begin( ), sorted_orders.end( ), by_angle);
			Run& run = *schedule.createRun(schedule.getRuns( ).end( ), depot_location, depot_location);
			while(!sorted_orders.empty( ))
			{
				int checkpoint = scene_editor.checkpoint( );
				scene_editor.performAction<AllocateOrder>(run, run.getWorkStops( ).end( ), sorted_orders.back( ));
				if(schedule.isValid( ))
				{
					sorted_orders.pop_back( );
					scene_editor.commit( );
				}
				else
				{
					scene_editor.rollbackToCheckpoint(checkpoint);
					break;
				}
			}
			scene_editor.commit( );

			if(sorted_orders.empty( ))
			{
				break;
			}
		}
	}
}
