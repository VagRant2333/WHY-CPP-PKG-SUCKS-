# cmake_minimum_required(VERSION 3.10)

# project(cal_project)

# set(CMAKE_CXX_STANDARD 17)

# find_package(fmt CONFIG REQUIRED)

# add_executable(cal src/cal.cpp)

# target_link_libraries(cal PRIVATE fmt::fmt)
cmake_minimum_required(VERSION 3.10)

project(cal_project)

set(CMAKE_CXX_STANDARD 23)

# Add fmt header files from the local fmt/include directory
include_directories(${CMAKE_SOURCE_DIR}/fmt/include)

add_executable(cal src/cal.cpp)

# Uncomment the line below if fmt requires linking (e.g., if it’s not header-only)
# target_link_libraries(cal PRIVATE ${CMAKE_SOURCE_DIR}/fmt/lib/libfmt.a)