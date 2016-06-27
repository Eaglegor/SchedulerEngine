add_library(GCBITSPSolver SHARED
	
	GCBITSPSolver.h GCBITSPSolver.cpp
    
    EdgeIntroducer.h
    EdgeSuggestor.h
    SuggestedEdge.h
    
    GCBIEdgeSuggestor.h GCBIEdgeSuggestor.cpp
    GCBIEdgeIntroducer.h GCBIEdgeIntroducer.cpp
    
    DirectEdgeIntroducer.h DirectEdgeIntroducer.cpp
    CircularEdgeIntroducer.h CircularEdgeIntroducer.cpp
)

set_target_properties(GCBITSPSolver PROPERTIES FOLDER "Engine/Algorithms/TSPSolvers/GCBI")

target_link_libraries(GCBITSPSolver SceneManager SceneEditor)

cotire(GCBITSPSolver)

include(GenerateExportHeader)
generate_export_header(GCBITSPSolver EXPORT_FILE_NAME ${CPP_SCHEDULER_BINARY_DIR}/SharedLibrariesExportHeaders/GCBITSPSolver_export.h)
