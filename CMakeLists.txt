cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Universal (arm64 + x86_64) is only meaningful for an actual macOS binary.
# Forcing it unconditionally here breaks iOS builds: this runs before
# project(), i.e. before Geode's own cmake/Platform.cmake gets a chance to
# correctly set CMAKE_OSX_ARCHITECTURES to just "arm64" for iOS - so
# project()'s first compiler check ends up trying to link against the
# iPhoneOS SDK (device-only, arm64 only, no x86_64 slice at all) while still
# asking for both architectures, which fails immediately.
if (NOT GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13 CACHE STRING "Minimum OS X deployment version")

project(LevelLink VERSION 1.0.0)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

if (NOT DEFINED ENV{GEODE_SDK})
	message(FATAL_ERROR "Unable to find Geode SDK! Set the GEODE_SDK environment variable to your Geode SDK path.")
else()
	message(STATUS "Found Geode SDK: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
