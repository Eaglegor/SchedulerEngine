#include "CWSavingsVRPSolver.h"

#include "SavingsStores/VectorSavingsStore.h"
#include "Savings/ClassicSaving.h"
#include "Generators/ClassicSavingsGenerator.h"
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/Order.h>
#include <Engine/SceneManager/Operation.h>
#include <Engine/SceneManager/Run.h>
#include <Engine/SceneManager/Stop.h>
#include <Engine/SceneManager/Schedule.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneEditor/Actions/CreateRun.h>
#include <Engine/SceneEditor/Actions/AllocateOrder.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include "CWSavingsPrechecks.h"

#include <iostream>

namespace Scheduler
{
	using SavingsStore = VectorSavingsStore<ClassicSaving>;
	using SavingsGenerator = ClassicSavingsGenerator;
	
	void CWSavingsVRPSolver::optimize(Scene* scene) const
	{
		if(!routing_service) return; // We can't calculate routes - so we can't optimize scene
		
		SavingsStore savings_list;
		SavingsGenerator generator(routing_service);
		generator.generateSavings(scene, savings_list);
		
		SceneEditor scene_editor;
		
		size_t index = 0;
		for(SavingsStore::value_type saving : savings_list)
		{
			std::cout << "Calculating " << std::to_string(index++) << " / " << std::to_string(savings_list.size()) << std::endl;
			Order* i = saving.i;
			Order* j = saving.j;
			
			int checkpoint = scene_editor.checkpoint();
			
			bool success = performAction(scene, scene_editor, i, j);
			
			if(success)
			{
				if(scene->isValid()) scene_editor.commit();
				else scene_editor.rollbackToCheckpoint(checkpoint);
			}
		};
		
		std::cout << "Finished" << std::endl;
	}

	bool CWSavingsVRPSolver::performAction(Scene* scene, SceneEditor& editor, Order* i, Order* j) const
	{
		auto i_stop_iter = scene->query().operationStopMapping().findWorkStop(i->getWorkOperation());
		auto j_stop_iter = scene->query().operationStopMapping().findWorkStop(j->getWorkOperation());
		
		if(!CWSavingsPrechecks::isValidSaving(i_stop_iter, j_stop_iter)) return false;
		
		WorkStop* i_stop = i_stop_iter ? *i_stop_iter.value() : nullptr;
		WorkStop* j_stop = j_stop_iter ? *j_stop_iter.value() : nullptr;
		
		if(!i_stop && !j_stop )
		{
			for(Schedule* schedule : scene->getSchedules())
			{
				if(schedule->getRuns().empty())
				{
					Schedule::RunsList::iterator run_iter = editor.performAction<CreateRun>(schedule, schedule->getRuns().end(), schedule->getPerformer()->getDepot()->getLocation(), schedule->getPerformer()->getDepot()->getLocation()).result().value();
					editor.performAction<AllocateOrder>(run_iter, (*run_iter)->getWorkStops().end(), i);
					editor.performAction<AllocateOrder>(run_iter, (*run_iter)->getWorkStops().end(), j);
				}
			}
		}
		if(!i_stop && j_stop )
		{
			Schedule::RunsList::const_iterator run_iter = util::find_iterator(j_stop->getRun()->getSchedule()->getRuns(), j_stop->getRun());
			editor.performAction<AllocateOrder>(run_iter, (*run_iter)->getWorkStops().begin(), i);
		}
		if(i_stop && !j_stop)
		{
			Schedule::RunsList::const_iterator run_iter = util::find_iterator(i_stop->getRun()->getSchedule()->getRuns(), i_stop->getRun());
			editor.performAction<AllocateOrder>(run_iter, (*run_iter)->getWorkStops().end(), j);
		}
		if(i_stop && j_stop )
		{
			Run* r = i_stop->getRun();
			// Append to tail
			// Remove j_stop run
		}
		
		return true;
	}
	
	void CWSavingsVRPSolver::setRoutingService(RoutingService* routing_service)
	{
		this->routing_service = routing_service;
	}

	
}