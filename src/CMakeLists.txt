set(APP_ICON_ICO "logo-128.ico")

add_executable(mytec_exe
  include/Mytec/util/str.hpp
  Mytec/util/str.cpp)

target_sources(mytec_exe PRIVATE
  Mytec/app.cpp
  Mytec/main.cpp
  Mytec/model/Image.cpp
  Mytec/model/editor.cpp
  Mytec/model/image_manager.cpp
  Mytec/tools/Pen.cpp
  Mytec/tools/View.cpp
  Mytec/ui/button.cpp
  Mytec/ui/dialogs.cpp
  Mytec/ui/message_box.cpp
  Mytec/ui/misc.cpp
  Mytec/ui/popup_modal.cpp
  Mytec/ui/style.cpp
  Mytec/util/file_dialog.cpp
)

target_include_directories(mytec_exe PRIVATE include)
target_link_libraries(mytec_exe PRIVATE SFML::System SFML::Window SFML::Graphics ImGui-SFML::ImGui-SFML nfd)
set_property(TARGET mytec_exe PROPERTY CXX_STANDARD 20)
add_definitions(-DPROJECT_VER="${PROJECT_VERSION}")

if (WIN32_EXECUTABLE)
  add_definitions(-DAPP_ICON_ICO="${APP_ICON_ICO}")
  target_sources(mytec_exe PRIVATE resources/resource.res)
endif ()

if (MSVC)
  target_compile_options(mytec_exe PRIVATE /W4 /WX /permissive-)
else ()
  target_compile_options(mytec_exe PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif ()

if (WIN32 AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET mytec_exe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:sfml-main>
    $<TARGET_FILE:sfml-system>
    $<TARGET_FILE:sfml-window>
    $<TARGET_FILE:sfml-graphics>
    $<TARGET_FILE:ImGui-SFML>
    $<TARGET_FILE_DIR:mytec_exe>
  )
endif ()

#add_custom_command(
#  TARGET mytec_exe POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
#  ${CMAKE_SOURCE_DIR}/assets
#  $<TARGET_FILE_DIR:mytec_exe>
#)