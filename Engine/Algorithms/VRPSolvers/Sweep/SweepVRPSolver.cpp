#ifdef _MSC_VER
#define _USE_MATH_DEFINES
#endif

#include "SweepVRPSolver.h"
#include <vector>
#include <algorithm>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Performer.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneManager/Location.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Order.h>

namespace Scheduler
{
    SweepVRPSolver::SweepVRPSolver()
	{
	}
    SweepVRPSolver::~SweepVRPSolver()
    {
    }

	void SweepVRPSolver::optimize(Scene* scene) const
	{
        struct ByAngle
        {
            ByAngle (const Site& center) : center_location(center)
            {}
            bool operator () (Operation* lhs, Operation* rhs) const
            {
                return angle(lhs) < angle(rhs);
            }
            float angle (Operation* operation) const
            {
                return angle(operation->getLocation().getSite());
            }
            float angle (const Site& location) const
            {
                const float x = (location.getLatitude() - center_location.getLatitude()).getValue();
                const float y = (location.getLongitude() - center_location.getLongitude()).getValue();
                const float hypot = std::hypot(x, y);
                const float angle = std::acos(y / hypot);
                return x > 0.f ? angle : 2* M_PI - angle;
            }
            float degreeAngle(const Site& location) const
            {
                return angle(location) * 180.f / M_PI;
            }

            Site center_location;
        };

        assert(scene);

        auto& schedules = scene->getSchedules();
        auto& orders = scene->getContext().getOrders();
        std::vector<Operation*> operations;
        for (auto order : orders) {
            auto& order_operations = order->getWorkOperations();
            for (auto operation : order_operations) {
                operations.push_back(operation);
            }
        }

        if (operations.empty()) {
            return;
        }

        for (auto schedule : schedules) {
            const Location &depot_location = schedule->getPerformer()->getDepot()->getLocation();
            ByAngle by_angle(depot_location.getSite());
            std::sort(operations.begin(), operations.end(), by_angle);
            Run* run = *schedule->createRun(schedule->getRuns().end(), depot_location, depot_location);
            while (!operations.empty() && schedule->isValid()) {
                auto iter = run->createWorkStop(run->getWorkStops().end(), operations.back());
                if (schedule->isValid()) {
                    operations.pop_back();
                } else {
                    run->destroyWorkStop(iter);
                    break;
                }
            }

            if (operations.empty()) {
                break;
            }
        }
	}
}
