add_library(OverloadRunValidationAlgorithm SHARED
	
	OverloadRunValidationAlgorithm.h
	OverloadRunValidationAlgorithm.cpp
)

set_target_properties(OverloadRunValidationAlgorithm PROPERTIES FOLDER "Engine/Algorithms/Validation/Run")

target_link_libraries(OverloadRunValidationAlgorithm SceneManager)

cotire(OverloadRunValidationAlgorithm)




InstallAlgorithm(OverloadRunValidationAlgorithm)
InstallHeaders(OverloadRunValidationAlgorithm.h)