set(TARGET_NAME EasyCutter)

add_executable(${TARGET_NAME} WIN32 main.cpp)

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
else()
  target_compile_options(${TARGET_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} fmt::fmt)

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(${TARGET_NAME} wx::core wx::base)
