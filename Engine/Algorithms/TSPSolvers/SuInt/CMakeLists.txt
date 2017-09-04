add_library(SuIntTSPSolver SHARED
	
	SuIntTSPSolver.h SuIntTSPSolver.cpp
    
    EdgeIntroducer.h
    EdgeSuggestor.h
    SuggestedEdge.h
    
    EdgeSuggestorType.h
    EdgeIntroducerType.h
    
    DistanceRatingEdgeSuggestor.h DistanceRatingEdgeSuggestor.cpp
    BetterEdgeSuggestor.h BetterEdgeSuggestor.cpp
    
    EdgeIntroducers/CompositeEdgeIntroducer.h EdgeIntroducers/CompositeEdgeIntroducer.cpp
    EdgeIntroducers/DirectEdgeIntroducer.h EdgeIntroducers/DirectEdgeIntroducer.cpp
    EdgeIntroducers/CircularEdgeIntroducer.h EdgeIntroducers/CircularEdgeIntroducer.cpp
    EdgeIntroducers/ReverseEdgeIntroducer.h EdgeIntroducers/ReverseEdgeIntroducer.cpp
)

set_target_properties(SuIntTSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/SuInt")

target_link_libraries(SuIntTSPSolver SceneManager SceneEditor LoggingManager)

cotire(SuIntTSPSolver)

include(GenerateExportHeader)
generate_export_header(SuIntTSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/SuIntTSPSolver_export.h)

InstallAlgorithm(SuIntTSPSolver)
InstallHeaders(
    SuIntTSPSolver.h
    EdgeSuggestorType.h
    EdgeIntroducerType.h
)