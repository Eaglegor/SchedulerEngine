add_library(ChainStopValidationAlgorithm SHARED
	
	ChainStopValidationAlgorithm.h
	ChainStopValidationAlgorithm.cpp
)

set_target_properties(ChainStopValidationAlgorithm PROPERTIES FOLDER "Engine/Algorithms/Validation/Stop")

target_link_libraries(ChainStopValidationAlgorithm SceneManager)

cotire(ChainStopValidationAlgorithm)




InstallAlgorithm(ChainStopValidationAlgorithm)
InstallHeaders(ChainStopValidationAlgorithm.h)