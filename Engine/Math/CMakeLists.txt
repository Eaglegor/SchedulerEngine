add_custom_target(Math SOURCES FloatEpsilon.h IntegerPower.h FixedPointMath.h)

set_target_properties(Math PROPERTIES FOLDER "Engine")

InstallHeaders(
    FloatEpsilon.h
    IntegerPower.h
    FixedPointMath.h
)