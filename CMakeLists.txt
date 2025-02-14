cmake_minimum_required (VERSION 3.15)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
	cmake_policy(SET CMP0141 NEW)
	set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("WADsworth" VERSION 1.0)

# Use C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

# I fucking hate CMake
add_subdirectory("${PROJECT_SOURCE_DIR}/vendored/SDL" EXCLUDE_FROM_ALL)

add_executable(WADsworth WIN32
	"${PROJECT_SOURCE_DIR}/src/Algorithm.c"
	"${PROJECT_SOURCE_DIR}/src/Error.c"
	"${PROJECT_SOURCE_DIR}/src/Files.c"
	"${PROJECT_SOURCE_DIR}/src/Main.c"
	"${PROJECT_SOURCE_DIR}/src/Math.c"
	"${PROJECT_SOURCE_DIR}/src/Memory.c"
	"${PROJECT_SOURCE_DIR}/src/WADStr.c")

target_include_directories(WADsworth PUBLIC "${PROJECT_SOURCE_DIR}/include")

target_link_libraries(WADsworth PRIVATE SDL3::SDL3)

if(MSVC)
	target_compile_options(WADsworth PRIVATE /W4 /WX)
else()
	target_compile_options(WADsworth PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
