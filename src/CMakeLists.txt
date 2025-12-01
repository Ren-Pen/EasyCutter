set(TARGET_NAME EasyCutter)

add_executable(${TARGET_NAME} WIN32 
${CMAKE_CURRENT_LIST_DIR}/main.cpp
${CMAKE_CURRENT_LIST_DIR}/pch.cpp
${CMAKE_CURRENT_LIST_DIR}/app/Events.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/ApplicationFrame.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/panel/VideoPanel.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/menu/MainMenuBar.cpp
${CMAKE_CURRENT_LIST_DIR}/ui/menu/MainMenuHandler.cpp
)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

target_precompile_headers(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/pch.h)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE fmt::fmt)

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE wx::core wx::base)

find_package(FFMPEG REQUIRED)

target_include_directories(${TARGET_NAME} PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${TARGET_NAME} PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE ${FFMPEG_LIBRARIES})