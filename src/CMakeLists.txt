add_executable(mytec-executable
  mytec/app.cpp
  mytec/asset.cpp
  mytec/file_dialog.cpp
  mytec/inline_assets.g.cpp
  mytec/main.cpp
  mytec/model/editor.cpp
  mytec/model/image_collection.cpp
  mytec/model/image.cpp
  mytec/model/tools/fill.cpp
  mytec/model/tools/pen.cpp
  mytec/model/tools/tool_t.cpp
  mytec/model/tools/view.cpp
  mytec/str.cpp
  mytec/ui/components/button.cpp
  mytec/ui/components/menu_bar.cpp
  mytec/ui/components/modal_stack.cpp
  mytec/ui/components/side_bar.cpp
  mytec/ui/components/status_bar.cpp
  mytec/ui/components/status_bar.cpp
  mytec/ui/components/dialogs/create_new_image.cpp
  mytec/ui/components/dialogs/message_box.cpp
  mytec/ui/components/dialogs/save_closing_image.cpp
  mytec/ui/event.cpp
  mytec/ui/misc.cpp
  mytec/ui/palette_tab.cpp
  mytec/ui/popup_modal.cpp
  mytec/ui/style.cpp
)

if(WIN32)
  set(MYTEC_APP_ICON_PATH "logo-128.ico")
else()
  set(MYTEC_APP_ICON_PATH "logo.png")
endif()


# Compile used assets into inline resources
find_package(Python COMPONENTS Interpreter)

set(RESOURCE_INLINER_INPUT
  -i ${PROJECT_SOURCE_DIR}/assets/logo-128.png main_icon
  -i ${PROJECT_SOURCE_DIR}/assets/icons/pen.png atlas_pen
  -i ${PROJECT_SOURCE_DIR}/assets/icons/view.png atlas_view
  -i ${PROJECT_SOURCE_DIR}/assets/pixChicago.ttf font_data
  -i ${PROJECT_SOURCE_DIR}/assets/palette-dos.act palette_dos
  -i ${PROJECT_SOURCE_DIR}/assets/palette-win.act palette_win
)
string(JOIN " " RESOURCE_INLINER_INPUT_FILES ${RESOURCE_INLINER_INPUT})
string(REGEX MATCHALL "-i [^ ]+ " RESOURCE_INLINER_INPUT_FILES "${RESOURCE_INLINER_INPUT_FILES}")
string(REGEX REPLACE "-i ([^ ]+) " "\\1" RESOURCE_INLINER_INPUT_FILES "${RESOURCE_INLINER_INPUT_FILES}")
set(RESOURCE_INLINER_OUTPUT "${PROJECT_SOURCE_DIR}/src/mytec/inline_assets.g.cpp")

add_custom_command(
  OUTPUT "${RESOURCE_INLINER_OUTPUT}"
  COMMAND ${Python_EXECUTABLE} ${PROJECT_SOURCE_DIR}/src/inline_resources.py
  -H "mytec/asset.hpp"
  -N "mytec::assets"
  -D "volatile const asset"
  -o "${RESOURCE_INLINER_OUTPUT}"
  ${RESOURCE_INLINER_INPUT}
  DEPENDS ${PROJECT_SOURCE_DIR}/src/inline_resources.py ${RESOURCE_INLINER_INPUT_FILES}
  VERBATIM
)
message(STATUS "Compiling inline assets done")

target_include_directories(mytec-executable PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(mytec-executable PRIVATE SFML::System SFML::Window SFML::Graphics ImGui-SFML::ImGui-SFML tinyfd)
target_precompile_headers(mytec-executable PRIVATE "${PROJECT_SOURCE_DIR}/include/mytec/PCH.hpp")
set_target_properties(mytec-executable PROPERTIES
  CXX_STANDARD 20
  OUTPUT_NAME mytec)
target_compile_definitions(mytec-executable PRIVATE MYTEC_PROJECT_VER="${PROJECT_VERSION}")

if(WIN32 AND NOT ("${CMAKE_BUILD_TYPE}" MATCHES "Debug"))
  target_compile_definitions(mytec-executable PRIVATE -DMYTEC_APP_ICON_PATH="${MYTEC_APP_ICON_PATH}" -DMYTEC_WIN32_EXECUTABLE)
  target_sources(mytec-executable PRIVATE mytec/assets/resource.res)
  set_target_properties(mytec-executable PROPERTIES WIN32_EXECUTABLE 1)
endif()

if(MSVC)
  target_compile_options(mytec-executable PRIVATE /W4 /WX /permissive-)
else()
  target_compile_options(mytec-executable PRIVATE -Wall -Wextra -Wpedantic -Werror -Winvalid-pch)
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET mytec-executable POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sfml-main>
    $<TARGET_FILE:sfml-system>
    $<TARGET_FILE:sfml-window>
    $<TARGET_FILE:sfml-graphics>
    $<TARGET_FILE:ImGui-SFML>
    $<TARGET_FILE_DIR:mytec-executable>
  )
endif()
