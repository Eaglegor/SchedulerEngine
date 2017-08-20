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
#include <Engine/SceneEditor/Actions/DestroyRun.h>
#include <Engine/SceneEditor/Actions/MoveWorkStops.h>
#include <Engine/SceneEditor/Actions/AllocateOrder.h>
#include <Engine/Utils/Collections/Algorithms.h>
#include "CWSavingsPrechecks.h"

#include <iostream>

namespace Scheduler
{
	using SavingsStore = VectorSavingsStore<ClassicSaving>;
	using SavingsGenerator = ClassicSavingsGenerator;
	
	void CWSavingsVRPSolver::optimize(Scene& scene) const
	{
		if(!routing_service) return;
		
		SavingsStore savings_list;
		SavingsGenerator generator(routing_service.get());
		generator.generateSavings(scene, savings_list);
		
		SceneEditor scene_editor;
		
		for(SavingsStore::value_type saving : savings_list)
		{
			const Order& i = saving.i;
			const Order& j = saving.j;
			
			int checkpoint = scene_editor.checkpoint();
			
			bool success = performAction(scene, scene_editor, i, j);
			
			if(success)
			{
				if(scene.isValid()){
					scene_editor.commit();
				}
				else 
				{
					scene_editor.rollbackToCheckpoint(checkpoint);
				}
			}
		};
	}

	bool CWSavingsVRPSolver::performAction(Scene& scene, SceneEditor& editor, const Order& i, const Order& j) const
	{
		auto i_stop_iter = scene.query().operationStopMapping().findWorkStop(i.getWorkOperation());
		auto j_stop_iter = scene.query().operationStopMapping().findWorkStop(j.getWorkOperation());
		
		if(!CWSavingsPrechecks::isValidSaving(i_stop_iter, j_stop_iter)) 
		{
			return false;
		}
		
		if(!i_stop_iter && !j_stop_iter )
		{
			for(Schedule& schedule : scene.getSchedules())
			{
				if(schedule.getRuns().empty())
				{
					Run& run = *editor.performAction<CreateRun>(schedule, schedule.getRuns().end(), schedule.getPerformer().constraints().depot().get().getLocation(), schedule.getPerformer().constraints().depot().get().getLocation());
					editor.performAction<AllocateOrder>(run, run.getWorkStops().end(), i);
					if(i != j) editor.performAction<AllocateOrder>(run, run.getWorkStops().end(), j);
					break;
				}
			}
		}
		if(!i_stop_iter && j_stop_iter )
		{
			Run& run = *j_stop_iter.get()->getRun().getSchedule().findRun(j_stop_iter.get()->getRun());
			editor.performAction<AllocateOrder>(run, run.getWorkStops().begin(), i);
		}
		if(i_stop_iter && !j_stop_iter)
		{
			Run& run = *i_stop_iter.get()->getRun().getSchedule().findRun(i_stop_iter.get()->getRun());
			editor.performAction<AllocateOrder>(run, run.getWorkStops().end(), j);
		}
		if(i_stop_iter && j_stop_iter )
		{
			Run& i_run = *i_stop_iter.get()->getRun().getSchedule().findRun(i_stop_iter.get()->getRun());
			Run& j_run = *j_stop_iter.get()->getRun().getSchedule().findRun(j_stop_iter.get()->getRun());
			
			editor.performAction<MoveWorkStops>(j_run, j_run.getWorkStops().begin(), j_run.getWorkStops().end(), i_run, i_run.getWorkStops().end());
			editor.performAction<DestroyRun>(j_run.getSchedule(), j_run.getSchedule().findRun(j_run));
		}
		
		return true;
	}
	
	void CWSavingsVRPSolver::setRoutingService(const RoutingService& routing_service)
	{
		this->routing_service = routing_service;
	}

	
}
