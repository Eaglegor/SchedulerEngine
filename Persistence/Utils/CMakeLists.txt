add_custom_target(PersistenceUtils SOURCES PtreeDeserializer.h)
set_target_properties(PersistenceUtils PROPERTIES FOLDER "Persistence/Utils")

InstallHeaders(PtreeDeserializer.h)