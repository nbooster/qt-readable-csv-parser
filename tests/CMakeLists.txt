cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(csv_parser_tests)

if (NOT TARGET GTest::gtest)
	message(FATAL_ERROR "Cannot find google test")
endif()
if (NOT TARGET csv_parser)
	message(FATAL_ERROR "Cannot find csv parser library")
endif()

enable_testing()

add_test(${PROJECT_NAME} ${PROJECT_NAME})

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
target_link_libraries(${PROJECT_NAME} PUBLIC Qt5::Core)
target_link_libraries(${PROJECT_NAME} PRIVATE csv_parser GTest::gtest)
