cmake_minimum_required(VERSION 3.16)

find_package(ZLIB REQUIRED)

add_executable(convert2
    convertisseur.cpp
    decode.cpp
    extraction_maps.cpp
    extraction_maps_conv.cpp
    extraction_sons.cpp
    extraction_sprites.cpp
    save_bmp.cpp
)

target_include_directories(convert2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(convert2 PRIVATE ZLIB::ZLIB)

# id_list lu depuis le répertoire de travail courant (cwd = decode/ ou build/decode)
add_custom_target(copy_id_list ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/id_list.txt
        ${CMAKE_CURRENT_BINARY_DIR}/id_list.txt.txt
    COMMENT "Copie id_list vers le répertoire de build decode"
)

add_dependencies(convert2 copy_id_list)
