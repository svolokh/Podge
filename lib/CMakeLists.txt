add_subdirectory(SDL2)
add_subdirectory(pugixml)

set(BOX2D_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_BUILD_UNITTESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(liquidfun/Box2D)
get_property(box2d_link_libs TARGET Box2D PROPERTY LINK_LIBRARIES)
list(REMOVE_ITEM box2d_link_libs rt)
set_property(TARGET Box2D PROPERTY LINK_LIBRARIES ${box2d_link_libs})
get_property(box2d_link_libs TARGET Box2D PROPERTY INTERFACE_LINK_LIBRARIES)
list(REMOVE_ITEM box2d_link_libs rt)
set_property(TARGET Box2D PROPERTY INTERFACE_LINK_LIBRARIES ${box2d_link_libs})

# nanovg
set(NANOVG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/nanovg)
add_library(nanovg SHARED
    ${NANOVG_DIR}/src/nanovg.c)
target_compile_definitions(nanovg PRIVATE $<$<CONFIG:Debug>:DEBUG> $<$<CONFIG:Release>:NDEBUG>)
target_include_directories(nanovg PRIVATE ${NANOVG_DIR}/src)
