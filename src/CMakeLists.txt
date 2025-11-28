set(TARGET_NAME EasyCutter)

add_executable(${TARGET_NAME} WIN32 
${CMAKE_CURRENT_LIST_DIR}/main.cpp
${CMAKE_CURRENT_LIST_DIR}/app/Events.h
${CMAKE_CURRENT_LIST_DIR}/app/Events.cpp
${CMAKE_CURRENT_LIST_DIR}/app/controller/VideoController.cpp
${CMAKE_CURRENT_LIST_DIR}/app/controller/VideoController.h
${CMAKE_CURRENT_LIST_DIR}/media/video/FFmpegDecoder.cpp
${CMAKE_CURRENT_LIST_DIR}/media/video/FFmpegDecoder.h
${CMAKE_CURRENT_LIST_DIR}/media/video/VideoFrame.h
${CMAKE_CURRENT_LIST_DIR}/ui/IDs.h
${CMAKE_CURRENT_LIST_DIR}/ui/ApplicationFrame.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/ApplicationFrame.h
${CMAKE_CURRENT_LIST_DIR}/ui/panel/VideoPanel.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/panel/VideoPanel.h
${CMAKE_CURRENT_LIST_DIR}/ui/menu/MainMenuBar.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/menu/MainMenuBar.h
${CMAKE_CURRENT_LIST_DIR}/ui/menu/MainMenuHandler.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/menu/MainMenuHandler.h
)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} fmt::fmt)

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} wx::core wx::base)
