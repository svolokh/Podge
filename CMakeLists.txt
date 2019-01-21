cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(podge C CXX)

option(PODGE_LIBRARY "compile Podge as a library" OFF)

if(IOS)
	function(podge_find_framework name outvar)
		find_library(${outvar} ${name})
		if(DEFINED ${${outvar}-NOTFOUND})
			message(FATAL_ERROR "Failed to find framework ${name}")
		endif()
	endfunction()
endif()

# place to copy headers to satisfy libraries' expectations about where headers should be
set(PODGE_HEADERS_MIRROR_DIR ${CMAKE_CURRENT_BINARY_DIR}/headers_mirror)
if(EXISTS ${PODGE_HEADERS_MIRROR_DIR})
	file(REMOVE_RECURSE ${PODGE_HEADERS_MIRROR_DIR})
endif()
file(MAKE_DIRECTORY ${PODGE_HEADERS_MIRROR_DIR})
include_directories(${PODGE_HEADERS_MIRROR_DIR})

add_subdirectory(lib)

set(PODGE_GENERATED_SOURCES_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
file(MAKE_DIRECTORY ${PODGE_GENERATED_SOURCES_DIR})

file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/aux/gen_systems.py PODGE_GEN_SYSTEMS)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/aux/gen_types.py PODGE_GEN_TYPES)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/podge/systems.json PODGE_SYSTEMS)
file(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/podge/types.json PODGE_TYPES)
file(TO_NATIVE_PATH ${PODGE_GENERATED_SOURCES_DIR}/systems.inl PODGE_SYSTEMS_INL)
file(TO_NATIVE_PATH ${PODGE_GENERATED_SOURCES_DIR}/types.inl PODGE_TYPES_INL)

set(PODGE_ROOT_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/podge/podge.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/podge/common.hpp)
set(PODGE_OS_HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/podge/os/resources.hpp)

if(NOT PODGE_LIBRARY)
	list(APPEND PODGE_OS_HEADERS
		${CMAKE_CURRENT_SOURCE_DIR}/podge/os/gfx.hpp)
endif()

set(PODGE_HEADERS ${PODGE_ROOT_HEADERS} ${PODGE_OS_HEADERS})

set(PODGE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/podge/podge.cpp
	${PODGE_SYSTEMS_INL}
	${PODGE_TYPES_INL})

add_custom_command(
	OUTPUT ${PODGE_SYSTEMS_INL}
	COMMAND python ${PODGE_GEN_SYSTEMS} ${PODGE_SYSTEMS} ${PODGE_SYSTEMS_INL}
	DEPENDS ${PODGE_SYSTEMS} ${PODGE_HEADERS} ${PODGE_GEN_SYSTEMS})

add_custom_command(
	OUTPUT ${PODGE_TYPES_INL}
	COMMAND python ${PODGE_GEN_TYPES} ${PODGE_TYPES} ${PODGE_TYPES_INL}
	DEPENDS ${PODGE_TYPES} ${PODGE_HEADERS} ${PODGE_GEN_TYPES})

if(ANDROID)
	list(APPEND PODGE_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/podge/os/gfx_opengles2.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/podge/os/android/resources.cpp)
elseif(IOS)
	list(APPEND PODGE_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/podge/os/gfx_opengles2.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/podge/os/ios/resources.mm)
else()
	if(NOT PODGE_LIBRARY)
		message(FATAL_ERROR "Podge can only be compiled with PODGE_LIBRARY on this platform.")
	endif()
	list(APPEND PODGE_SOURCES
		${CMAKE_CURRENT_SOURCE_DIR}/podge/os/generic/resources.cpp)
endif()

if(NOT PODGE_LIBRARY)
	list(APPEND PODGE_SOURCES 
		${CMAKE_CURRENT_SOURCE_DIR}/podge/main.cpp)
endif()

if(PODGE_LIBRARY)
	add_library(podge STATIC ${PODGE_SOURCES})
else()
	if(ANDROID)
		add_library(podge SHARED ${PODGE_SOURCES})
		set_target_properties(podge PROPERTIES
			DEBUG_POSTFIX "")
	elseif(IOS)
		add_executable(podge ${PODGE_SOURCES})
	else()
		message(FATAL_ERROR)
	endif()
endif()

set_property(TARGET podge PROPERTY CXX_STANDARD 14)
target_include_directories(podge PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/podge
	${CMAKE_CURRENT_BINARY_DIR}/generated
	${CMAKE_CURRENT_SOURCE_DIR}/lib/easy_profiler/easy_profiler_core/include
	${CMAKE_CURRENT_SOURCE_DIR}/lib/glm
	${CMAKE_CURRENT_SOURCE_DIR}/lib/pugixml/src
	${CMAKE_CURRENT_SOURCE_DIR}/lib/boost
	${CMAKE_CURRENT_SOURCE_DIR}/lib/liquidfun/Box2D
	${CMAKE_CURRENT_SOURCE_DIR}/lib/json/single_include
	${CMAKE_CURRENT_SOURCE_DIR}/lib/nanovg/src
	${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2_mixer)
target_link_libraries(podge pugixml Box2D nanovg SDL2-static SDL2_mixer)

if(NOT PODGE_LIBRARY)
	if(ANDROID)
		target_link_libraries(podge GLESv2)
	elseif(IOS)
		podge_find_framework(OpenGLES PODGE_OPENGLES_FRAMEWORK)
		podge_find_framework(AVFoundation PODGE_AVFOUNDATION_FRAMEWORK)
		podge_find_framework(UIKit PODGE_UIKIT_FRAMEWORK)
		podge_find_framework(CoreMotion PODGE_COREMOTION_FRAMEWORK)
		podge_find_framework(CoreGraphics PODGE_COREGRAPHICS_FRAMEWORK)
		podge_find_framework(GameController PODGE_GAMECONTROLLER_FRAMEWORK)
		podge_find_framework(QuartzCore PODGE_QUARTZCORE_FRAMEWORK)
		target_link_libraries(podge ${PODGE_OPENGLES_FRAMEWORK} ${PODGE_AVFOUNDATION_FRAMEWORK} ${PODGE_UIKIT_FRAMEWORK} ${PODGE_COREMOTION_FRAMEWORK} ${PODGE_GAMECONTROLLER_FRAMEWORK} ${PODGE_QUARTZCORE_FRAMEWORK} ${PODGE_COREGRAPHICS_FRAMEWORK})
	else()
		message(FATAL_ERROR)
	endif()

	target_include_directories(podge PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/lib/SDL2/include
		${CMAKE_CURRENT_SOURCE_DIR}/lib/nuklear)
	
	if(ANDROID)
		target_link_libraries(podge SDL2main)
	endif()

	if(ANDROID OR IOS)
		target_include_directories(podge PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/lib/nuklear/demo/sdl_opengles2)
	else()
		message(FATAL_ERROR)
	endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(podge PRIVATE DEBUG=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions(podge PRIVATE NDEBUG=1)
	target_compile_options(podge PRIVATE -O3)
endif()

target_compile_options(podge PRIVATE -Werror -Wall -Wno-logical-op-parentheses -Wno-unused-local-typedef -Wno-unused-variable -Wno-error-unused-local-typedef -Wno-error-unused-function)

if(PODGE_LIBRARY)
	install(TARGETS podge
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
	install(FILES ${PODGE_ROOT_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/podge)
	install(FILES ${PODGE_OS_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/podge/os)
endif()
