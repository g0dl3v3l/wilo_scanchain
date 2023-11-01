set(SOURCES src/scanchain_spi/scanchain_spi.cpp)
            
idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS include
                    REQUIRES  cppspi)