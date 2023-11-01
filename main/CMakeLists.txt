set(SOURCES main.cpp)
set(COMPONENT_ADD_INCLUDEDIRS "")
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS .)

