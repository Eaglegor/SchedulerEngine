#include "RandomSolutionGenerator.h"
#include <Engine/SceneManager/Scene.h>
#include <Engine/SceneManager/SceneContext.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/SceneManager/Depot.h>
#include <Engine/SceneEditor/SceneEditor.h>
#include <Engine/SceneEditor/Actions/AllocateOrder.h>
#include <random>
#include <numeric>
#include <algorithm>

namespace Scheduler 
{
	Scene& RandomSolutionGenerator::generate(Scene& base_scene)
	{
		const SceneContext& scene_context = base_scene.getContext();
		SceneManager& scene_manager = base_scene.getSceneManager();
		
		Scene& scene = scene_manager.createScene(scene_context);
		
		std::vector<std::size_t> orders(scene_context.getOrders().size());
		orders.resize(scene_context.getOrders().size());
		std::iota(orders.begin(), orders.end(), 0);
		
		std::random_device rd;
		std::mt19937 g(rd());
		
		std::shuffle(orders.begin(), orders.end(), g);
		
		std::vector<std::size_t> assignments(base_scene.getSchedules().size());
		for(int i = 0; i < assignments.size(); ++i)
		{
			assignments.push_back(g() % orders.size());
		}
		
		std::sort(assignments.begin(), assignments.end());
		
		SceneEditor scene_editor;
		
		for(int i = 0; i < assignments.size() - 1; ++i)
		{
			std::size_t range_begin = assignments[i];
			std::size_t range_end = assignments[i+1];
			
			if(range_end <= range_begin) continue;
			
			Schedule& current_schedule = scene.getSchedules()[i];
			
			const Location& depot_location = current_schedule.getPerformer().getDepot()->getLocation();
			Run& current_run = *current_schedule.createRun(current_schedule.getRuns().end(), depot_location, depot_location);
			
			for(int j = range_begin; j < range_end; ++j)
			{
				scene_editor.performAction<AllocateOrder>(current_run, current_run.getWorkStops().end(), scene_context.getOrders()[orders[j]]);
			}
			
			scene_editor.commit();
		}
		
		
		
		return scene;
	}
}