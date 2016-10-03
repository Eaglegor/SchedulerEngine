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
#include "CWSavingsPrechecks.h"


namespace Scheduler
{
	using SavingsStore = VectorSavingsStore<ClassicSaving>;
	using SavingsGenerator = ClassicSavingsGenerator;
	
	void CWSavingsVRPSolver::optimize(Scene* scene) const
	{
		SavingsStore savings_list;
		SavingsGenerator::generateSavings(scene, savings_list);
		
		SceneEditor scene_editor;
		
		for(SavingsStore::value_type saving : savings_list)
		{
			Order* i = saving.i;
			Order* j = saving.j;
			
			int checkpoint = scene_editor.checkpoint();
			
			bool success = performAction(scene, i, j);
			
			if(success)
			{
				if(scene->isValid()) scene_editor.commit();
				else scene_editor.rollbackToCheckpoint(checkpoint);
			}
		};
	}

	bool CWSavingsVRPSolver::performAction(Scene* scene, Order* i, Order* j) const
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
					// Create run
				}
			}
		}
		if(!i_stop && j_stop )
		{
			Run* r = j_stop->getRun();
			// Append to head
		}
		if(i_stop && !j_stop)
		{
			Run* r = i_stop->getRun();
			// Append to tail
		}
		if(i_stop && j_stop )
		{
			Run* r = i_stop->getRun();
			// Append to tail
			// Remove j_stop run
		}
		
		return true;
	}
	
}