#include "SweepVRPSolver.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Schedule.h>
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
            ByAngle (const Location& center) : center_location(center)
            {}
            bool operator () (Operation* lhs, Operation* rhs) const
            {
                return angle(lhs) < angle(rhs);
            }
            float angle (Operation* operation) const
            {
                return angle(operation->getLocation());
            }
            float angle (const Location& location) const
            {
                const float x = (location.getLatitude() - center_location.getLatitude()).getValue();
                const float y = (location.getLongitude() - center_location.getLongitude()).getValue();
                if (y == 0.f) {
                    return 0.f;
                }
                const float ctg = x / y;
                return M_PI_2 - std::atan(ctg);
            }

            Location center_location;
        };
        assert(scene);

        auto& schedules = scene->getSchedules();
        auto& orders = scene->getOrders();
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
            Location depot_location = schedule->getDepotLocation();
            ByAngle by_angle(depot_location);
            std::sort(operations.begin(), operations.end(), by_angle);

            Run* run = schedule->createRun(depot_location, depot_location);
            while (!operations.empty() && schedule->isValid()) {
                run->allocateWorkOperation(operations.back());
                if (schedule->isValid()) {
                    operations.pop_back();
                } else {
                    run->unallocateWorkOperation(operations.back());
                    break;
                }
            }

            if (operations.empty()) {
                break;
            }
        }
	}
}