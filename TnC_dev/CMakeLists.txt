cmake_minimum_required(VERSION 3.16)

find_package(SDL3 REQUIRED CONFIG)

find_package(SDL3_image QUIET CONFIG)
if(NOT SDL3_image_FOUND)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL3_IMAGE QUIET SDL3_image)
    endif()
    if(SDL3_IMAGE_FOUND)
        add_library(SDL3_image::SDL3_image INTERFACE IMPORTED)
        target_include_directories(SDL3_image::SDL3_image INTERFACE ${SDL3_IMAGE_INCLUDE_DIRS})
        target_link_libraries(SDL3_image::SDL3_image INTERFACE ${SDL3_IMAGE_LIBRARIES})
    else()
        include(FetchContent)
        FetchContent_Declare(
            SDL3_image
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
            GIT_TAG release-3.2.4
        )
        FetchContent_MakeAvailable(SDL3_image)
    endif()
endif()

set(TNC_CORE_SOURCES
    FontManager/fontmanager.cpp
    MapInterface/mapinterface.cpp
    MapInterface/mapi_full_redraw.cpp
    MapInterface/mapi_get_map.cpp
    MapInterface/mapi_move_map.cpp
    TextManager/textmanager.cpp
    NPCManager/npc_ajout.cpp
    NPCManager/npc_config.cpp
    NPCManager/npc_draw.cpp
    NPCManager/npcmanager.cpp
    VSFInterface/vsfi_create_hash.cpp
    VSFInterface/vsfi_hash.cpp
    VSFInterface/vsfi_indexage_pal.cpp
    VSFInterface/vsfi_indexage_rep.cpp
    VSFInterface/vsfi_indexage_spr.cpp
    VSFInterface/vsfi_palettes.cpp
    VSFInterface/vsfi_read_sprite.cpp
    VSFInterface/vsfi_save_bmp.cpp
    VSFInterface/vsfi_sprites.cpp
    VSFInterface/vsfinterface.cpp
    render/Sdl3FramePresenter.cpp
    test_mapinterface_sdl3.cpp
)

add_executable(mapi_sdl3 ${TNC_CORE_SOURCES})

target_include_directories(mapi_sdl3 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/render
)

# SDL3 natif — plus de shim SDL/SDL.h

target_link_libraries(mapi_sdl3 PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
)

# Assets copiés à côté de l'exécutable pour chemins relatifs ./fonts, torche.png
add_custom_command(TARGET mapi_sdl3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/fonts
        $<TARGET_FILE_DIR:mapi_sdl3>/fonts
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/modif_sprites.txt
        $<TARGET_FILE_DIR:mapi_sdl3>/modif_sprites.txt
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/NPCList.txt
        $<TARGET_FILE_DIR:mapi_sdl3>/NPCList.txt
)

foreach(ASSET torche.png LoadingScreen.bmp)
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET})
        add_custom_command(TARGET mapi_sdl3 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/${ASSET}
                $<TARGET_FILE_DIR:mapi_sdl3>/${ASSET}
        )
    endif()
endforeach()

# Données converties (sprites/maps) — lien vers ../../data depuis le build dir
if(EXISTS ${CMAKE_SOURCE_DIR}/data)
    add_custom_command(TARGET mapi_sdl3 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rm -f $<TARGET_FILE_DIR:mapi_sdl3>/data
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:mapi_sdl3>/data
    )
endif()
