cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
project(podge C CXX)

if(NOT ANDROID)
	message(FATAL_ERROR "This platform is not supported.")
endif()

add_subdirectory(lib)

option(PODGE_MAIN_ENABLED "whether to compile with podge()" TRUE)

set(PODGE_GENERATED_SOURCES_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY "${PODGE_GENERATED_SOURCES_DIR}")

file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../util/gen_systems.py" PODGE_GEN_SYSTEMS)
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../util/gen_types.py" PODGE_GEN_TYPES)
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/podge/systems.json" PODGE_SYSTEMS)
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/podge/types.json" PODGE_TYPES)
file(TO_NATIVE_PATH "${PODGE_GENERATED_SOURCES_DIR}/systems.inl" PODGE_SYSTEMS_INL)
file(TO_NATIVE_PATH "${PODGE_GENERATED_SOURCES_DIR}/types.inl" PODGE_TYPES_INL)

add_custom_command(
	OUTPUT "${PODGE_SYSTEMS_INL}" 
	COMMAND python "${PODGE_GEN_SYSTEMS}" "${PODGE_SYSTEMS}" "${PODGE_SYSTEMS_INL}"
	DEPENDS "${PODGE_SYSTEMS}")

add_custom_command(
	OUTPUT "${PODGE_TYPES_INL}"
	COMMAND python "${PODGE_GEN_TYPES}" "${PODGE_TYPES}" "${PODGE_TYPES_INL}"
	DEPENDS "${PODGE_TYPES}")

set(PODGE_SOURCES
	podge/podge.cpp
	podge/os/android/gfx.cpp
	podge/os/android/resources.cpp
	"${PODGE_SYSTEMS_INL}"
	"${PODGE_TYPES_INL}")

if(PODGE_MAIN_ENABLED)
	list(APPEND PODGE_SOURCES 
		podge/main.cpp)
endif()

add_library(podge SHARED ${PODGE_SOURCES})

set_property(TARGET podge PROPERTY CXX_STANDARD 14)
target_include_directories(podge PRIVATE 
	podge
	${CMAKE_CURRENT_BINARY_DIR}/generated
	lib/easy_profiler/easy_profiler_core/include
	lib/glm
	lib/pugixml/src
	lib/boost
	lib/liquidfun/Box2D
	lib/json/single_include
	lib/nanovg/src)
target_link_libraries(podge GLESv2 pugixml Box2D nanovg)

if(PODGE_MAIN_ENABLED)
	target_include_directories(podge PRIVATE
		lib/SDL2/include)
	target_link_libraries(podge SDL2main SDL2)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(podge PRIVATE DEBUG=1 _LIBCPP_DEBUG=1)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
	target_compile_definitions(podge PRIVATE NDEBUG=1 _LIBCPP_DEBUG=0)
	target_compile_options(podge PRIVATE -O3)
endif()

target_compile_options(podge PRIVATE -Werror -Wall -Wno-unused-variable -Wno-unused-local-typedef -Wno-logical-op-parentheses -Wno-error-unused-function)
