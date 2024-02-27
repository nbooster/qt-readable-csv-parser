cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(csv_parser VERSION 1.0 LANGUAGES CXX)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
  include/csv_parser/csvdocument.h
  src/csvparser.h
  src/csvparser.cpp
  src/csvdocument.cpp
)

add_library(${PROJECT_NAME} ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOUIC ON
    AUTOMOC ON
    AUTORCC ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(csv_parser PRIVATE Qt5::Core)

target_include_directories(csv_parser PUBLIC include PRIVATE src)

add_subdirectory(3rd-party/googletest)
add_subdirectory(tests)
