# Source files in src 
set(TT_FILES ticktoucan_platform.cpp)


include_directories(${CMAKE_SOURCE_DIR}/../include/platform_contracts)


# Create the main library from src 
add_library(ticktoucanplatform SHARED ${TT_FILES})

target_link_libraries(ticktoucanplatform ${TICKTOUCAN_PLATFORM_LINK_LIBRARIES})

install(TARGETS ticktoucanplatform
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)