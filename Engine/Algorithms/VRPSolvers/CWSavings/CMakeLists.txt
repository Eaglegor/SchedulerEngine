add_library(CWSavingsVRPSolver SHARED
	
	CWSavingsVRPSolver.h CWSavingsVRPSolver.cpp
	
	Generators/ClassicSavingsGenerator.h
	
	Savings/ClassicSaving.h
	
	SavingsStores/VectorSavingsStore.h
)

set_target_properties(CWSavingsVRPSolver PROPERTIES FOLDER "Engine/Algorithms/VRPSolvers/CWSavings")

target_link_libraries(CWSavingsVRPSolver StrategiesManager SceneManager SceneEditor)

cotire(CWSavingsVRPSolver)

include(GenerateExportHeader)
generate_export_header(CWSavingsVRPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/CWSavingsVRPSolver_export.h)

