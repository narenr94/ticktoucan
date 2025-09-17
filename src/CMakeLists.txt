# Source files in src 
set(TT_FILES ticktoucan.cpp)

pkg_check_modules(TICKTOUCAN_PLATFORM REQUIRED libticktoucanplatform)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/platform_contracts)



# Create the main library from src 
add_library(ticktoucan SHARED ${TT_FILES})
target_include_directories(ticktoucan PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(ticktoucan ${TICKTOUCAN_PLATFORM_LINK_LIBRARIES})

install(TARGETS ticktoucan
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

install(FILES ${CMAKE_SOURCE_DIR}/include/ticktoucan.h ${CMAKE_SOURCE_DIR}/include/platform_contracts/ticktoucan_platform.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)