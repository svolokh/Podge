include(GNUInstallDirs)

function(podge_check_submodule name path)
	if(NOT EXISTS ${path})
		message(FATAL_ERROR "missing submodule '${name}', please run `git submodule update --init --recursive`")
	endif()
endfunction()

# SDL2
set(SDL_SHARED_ENABLED_BY_DEFAULT OFF CACHE BOOL "" FORCE)
set(SDL_STATIC_PIC ON CACHE BOOL "" FORCE)
if(IOS)
	set(SDL_SKIP_CONFIGURE ON CACHE BOOL "" FORCE) # we will copy in SDL_config_iphoneos.h ourselves
endif()
podge_check_submodule(nuklear ${CMAKE_CURRENT_SOURCE_DIR}/nuklear/nuklear.h)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SDL2)
if(IOS)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include/SDL_config_iphoneos.h DESTINATION ${SDL2_BINARY_DIR}/include)
	file(RENAME ${SDL2_BINARY_DIR}/include/SDL_config_iphoneos.h ${SDL2_BINARY_DIR}/include/SDL_config.h)
endif()
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include DESTINATION ${PODGE_HEADERS_MIRROR_DIR})
file(RENAME ${PODGE_HEADERS_MIRROR_DIR}/include ${PODGE_HEADERS_MIRROR_DIR}/SDL2)
file(COPY ${SDL2_BINARY_DIR}/include/SDL_config.h DESTINATION ${PODGE_HEADERS_MIRROR_DIR}/include/SDL2)

# SDL2_mixer's libogg
set(OGG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer/external/libogg-1.3.2)
set(OGG_SOURCE_FILES
	${OGG_DIR}/src/bitwise.c
	${OGG_DIR}/src/framing.c)
file(MAKE_DIRECTORY ${PODGE_HEADERS_MIRROR_DIR}/ogg)
file(COPY ${CMAKE_SOURCE_DIR}/aux/ogg_config_types.h DESTINATION ${PODGE_HEADERS_MIRROR_DIR}/ogg)
file(RENAME ${PODGE_HEADERS_MIRROR_DIR}/ogg/ogg_config_types.h ${PODGE_HEADERS_MIRROR_DIR}/ogg/config_types.h)
add_library(ogg STATIC ${OGG_SOURCE_FILES})
target_include_directories(ogg PRIVATE ${OGG_DIR}/include)

# SDL2_mixer's libvorbis
set(VORBIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer/external/libvorbis-1.3.5)
set(VORBIS_SOURCE_FILES 
	${VORBIS_DIR}/lib/vorbisenc.c
	${VORBIS_DIR}/lib/res0.c
	${VORBIS_DIR}/lib/block.c
	${VORBIS_DIR}/lib/lookup.c
	${VORBIS_DIR}/lib/barkmel.c
	${VORBIS_DIR}/lib/info.c
	${VORBIS_DIR}/lib/mdct.c
	${VORBIS_DIR}/lib/tone.c
	${VORBIS_DIR}/lib/envelope.c
	${VORBIS_DIR}/lib/analysis.c
	${VORBIS_DIR}/lib/codebook.c
	${VORBIS_DIR}/lib/floor0.c
	${VORBIS_DIR}/lib/bitrate.c
	${VORBIS_DIR}/lib/psy.c
	${VORBIS_DIR}/lib/smallft.c
	${VORBIS_DIR}/lib/sharedbook.c
	${VORBIS_DIR}/lib/floor1.c
	${VORBIS_DIR}/lib/synthesis.c
	${VORBIS_DIR}/lib/mapping0.c
	${VORBIS_DIR}/lib/lpc.c
	${VORBIS_DIR}/lib/lsp.c
	${VORBIS_DIR}/lib/registry.c
	${VORBIS_DIR}/lib/vorbisfile.c
	${VORBIS_DIR}/lib/window.c)
add_library(vorbis STATIC ${VORBIS_SOURCE_FILES})
target_include_directories(vorbis PRIVATE ${VORBIS_DIR}/include ${VORBIS_DIR}/lib ${OGG_DIR}/include)
target_link_libraries(vorbis PUBLIC ogg)

# SDL2_mixer
set(SDL2_MIXER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/SDL2_mixer)
set(SDL2_MIXER_SOURCE_FILES
	${SDL2_MIXER_DIR}/effect_position.c
	${SDL2_MIXER_DIR}/effect_stereoreverse.c
	${SDL2_MIXER_DIR}/effects_internal.c
	${SDL2_MIXER_DIR}/load_aiff.c
	${SDL2_MIXER_DIR}/load_voc.c
	${SDL2_MIXER_DIR}/mixer.c
	${SDL2_MIXER_DIR}/music.c
	${SDL2_MIXER_DIR}/music_ogg.c
	${SDL2_MIXER_DIR}/playmus.c
	${SDL2_MIXER_DIR}/playwave.c)
add_library(SDL2_mixer STATIC ${SDL2_MIXER_SOURCE_FILES})
target_compile_definitions(SDL2_mixer PRIVATE MUSIC_OGG)
target_include_directories(SDL2_mixer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include ${VORBIS_DIR}/include ${OGG_DIR}/include)
target_link_libraries(SDL2_mixer PUBLIC SDL2-static vorbis)

# pugixml
add_subdirectory(pugixml)

# Box2D
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
