add_library(ChainRunValidationAlgorithm SHARED
	
	ChainRunValidationAlgorithm.h
	ChainRunValidationAlgorithm.cpp
)

set_target_properties(ChainRunValidationAlgorithm PROPERTIES FOLDER "Engine/Algorithms/Validation/Run")

target_link_libraries(ChainRunValidationAlgorithm SceneManager)

cotire(ChainRunValidationAlgorithm)




InstallAlgorithm(ChainRunValidationAlgorithm)
InstallHeaders(ChainRunValidationAlgorithm.h)