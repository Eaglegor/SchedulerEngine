add_custom_target(Utils SOURCES
	Units/DurationUnits.h
	Units/SpeedUnits.h
	
	Collections/Algorithms.h
	Collections/ImmutableUnorderedSet.h
	Collections/ImmutableVector.h

	Collections/Range.h
	Collections/RangeTypeAdapter.h
	
	Collections/LinkedPointersList.h
	Collections/LinkedPointersSublist.h
	
    Aggregators/DistanceAccumulator.h
    Aggregators/DurationAccumulator.h
    
	Factory.h
    CallableVisitorProxy.h
)

set_target_properties(Utils PROPERTIES FOLDER "Engine")

include_directories(${BOOST_ROOT})
