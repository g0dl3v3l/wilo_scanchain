set(SOURCES src/cppspi/cppspi.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include
                    REQUIRES  driver)