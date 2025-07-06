set(SFML_BUILD_AUDIO OFF)
set(SFML_BUILD_NETWORK OFF)
set(SFML_ENABLE_PCH ON)

add_subdirectory(sfml)

## set directory relative to this file
set(IMGUI_DIR ../imgui)
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_ENABLE_WARNINGS ON)

add_subdirectory(imgui-sfml)
add_subdirectory(tinyfd)