cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN32 0)

project(cherryhex)

set(chex_VERSION_MAJOR 0)
set(chex_VERSION_MINOR 1)
set(chex_VERSION_PATCH 0)

set(chex_VERSION_EXTRA "")

set(chex_VERSION "${chex_VERSION_MAJOR}.${chex_VERSION_MINOR}.${chex_VERSION_PATCH}")

if ( chex_VERSION_EXTRA )
	set(chex_VERSION "${chex_VERSION}-${chex_VERSION_EXTRA}")
endif ()

message("Conifguring cherryhex version ${chex_VERSION}.")

if( MSVC )
	set(dialect "")
	set(warnings "")
	set(flags "/EHsc /MP")

	set(chex_DEBUG_FLAGS "/Od")
	set(chex_RELEASE_FLAGS "/Ox")
else ()
	set(dialect "-std=c++1y")
	set(warnings "-Wall -Wextra")
	
	set(chex_DEBUG_FLAGS "-O0 -g3")
	set(chex_RELEASE_FLAGS "-O3 -g0 -march=native")
	
	if ( DEFINED WIN32 )
		# MinGW-w64
		set(flags "-mwindows -mconsole")
		set(OS_LIBS Version Imm32 winmm mingw32 gdi32)
	else ()
		# GCC/Clang
		set(flags "-fPIC")
		set(OS_LIBS "")
	endif ()
endif()

file(GLOB_RECURSE chex_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/tinyfiledialogs.c)
file(GLOB_RECURSE chex_HEADERS ${PROJECT_SOURCE_DIR}/include/*.hpp)

configure_file (
	"${PROJECT_SOURCE_DIR}/gen/cherryhex_version.hpp.in"
	"${PROJECT_BINARY_DIR}/cherryhex_version.hpp"
)

set(CMAKE_CXX_FLAGS_BASE "${CMAKE_CXX_FLAGS_BASE} ${dialect} ${warnings} ${flags}")

set(EXECUTABLE_NAME "cherryhex")

if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set(CMAKE_CXX_FLAGS_BASE "${CMAKE_CXX_FLAGS_BASE} ${chex_DEBUG_FLAGS}")
else ()
    set(CMAKE_CXX_FLAGS_BASE "${CMAKE_CXX_FLAGS_BASE} ${chex_RELEASE_FLAGS}")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_BASE}")

include_directories("include" ${PROJECT_BINARY_DIR})

set(LIBS "")

message("CXX Flags: ${CMAKE_CXX_FLAGS}")

add_executable(${EXECUTABLE_NAME} ${chex_SOURCES} ${chex_HEADERS})
target_link_libraries(${EXECUTABLE_NAME} ${LIBS})
