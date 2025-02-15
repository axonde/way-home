add_subdirectory(utils)
add_subdirectory(config)
add_subdirectory(finder)
add_subdirectory(bin)

target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(config PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(finder PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})