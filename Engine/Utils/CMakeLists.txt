add_custom_target(Utils SOURCES
	Units/DurationUnits.h
	Units/SpeedUnits.h
	
	Collections/Algorithms.h
	Collections/ImmutableUnorderedSet.h
	Collections/ImmutableVector.h
	
	Factory.h
    CallableVisitorProxy.h
)

set_target_properties(Utils PROPERTIES FOLDER "Engine")