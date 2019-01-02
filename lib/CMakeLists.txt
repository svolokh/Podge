include(GNUInstallDirs)

if(NOT PODGE_LIBRARY)
    add_subdirectory(SDL2)
endif()

add_subdirectory(pugixml)

set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNITTESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(liquidfun/Box2D)

get_property(box2d_link_libs TARGET Box2D PROPERTY LINK_LIBRARIES)
list(FIND box2d_link_libs rt box2d_rt_idx)
if(NOT box2d_rt_idx EQUAL -1)
    list(REMOVE_AT box2d_link_libs ${box2d_rt_idx})
endif()
set_property(TARGET Box2D PROPERTY LINK_LIBRARIES ${box2d_link_libs})

get_property(box2d_link_libs TARGET Box2D PROPERTY INTERFACE_LINK_LIBRARIES)
list(FIND box2d_link_libs rt box2d_rt_idx)
if(NOT box2d_rt_idx EQUAL -1)
    list(REMOVE_AT box2d_link_libs ${box2d_rt_idx})
endif()
set_property(TARGET Box2D PROPERTY INTERFACE_LINK_LIBRARIES ${box2d_link_libs})

# nanovg
set(NANOVG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nanovg)
add_library(nanovg STATIC
    ${NANOVG_DIR}/src/nanovg.c)
target_compile_definitions(nanovg PRIVATE $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG>)
target_include_directories(nanovg PRIVATE ${NANOVG_DIR}/src)
install(FILES ${NANOVG_DIR}/src/nanovg.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS nanovg ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

# nlohmann json
set(JSON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/json)
install(FILES ${JSON_DIR}/single_include/json.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
