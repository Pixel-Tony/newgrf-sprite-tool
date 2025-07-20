cmake_minimum_required(VERSION 3.12)
project(mytec
  LANGUAGES C CXX
  VERSION 1.0.0)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.9.1 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()
qt_add_executable(mytec-executable
  resources.qrc
  src/main.cpp
  src/model/image.cpp
  src/model/image.hpp
  src/model/palette_action.cpp
  src/model/palette_action.hpp
  src/model/palette.cpp
  src/model/palette.hpp
  src/model/tools/pen.hpp
  src/model/tools/pen.cpp
  src/model/tools/tool_factory.cpp
  src/model/tools/tool_factory.hpp
  src/model/tools/tool.cpp
  src/model/tools/tool.hpp
  src/model/tools/view.cpp
  src/model/tools/view.hpp
  src/util/str.cpp
  src/util/str.hpp
  src/widgets/canvas.cpp
  src/widgets/canvas.hpp
  src/widgets/dialogs/create_file_dialog.cpp
  src/widgets/dialogs/create_file_dialog.hpp
  src/widgets/dialogs/save_file_dialog.cpp
  src/widgets/dialogs/save_file_dialog.hpp
  src/widgets/editor.cpp
  src/widgets/editor.hpp
  src/widgets/main_window.cpp
  src/widgets/main_window.hpp
  src/widgets/palette_tab.cpp
  src/widgets/palette_tab.hpp
)

set_target_properties(mytec-executable PROPERTIES
  CXX_STANDARD 20
  OUTPUT_NAME mytec
)

target_include_directories(mytec-executable PRIVATE src)
target_precompile_headers(mytec-executable PRIVATE src/headers.hpp)
target_link_libraries(mytec-executable PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

target_compile_options(mytec-executable
  PRIVATE $<$<STREQUAL:CMAKE_CXX_COMPILER_ID,MSVC>:"/W4 /WX /permissive-">
  PRIVATE $<$<STREQUAL:CMAKE_CXX_COMPILER_ID,GNU>:"-Wall -Wextra -Wpedantic -Werror">
)
