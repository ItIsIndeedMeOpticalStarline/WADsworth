cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("WADsworth Tests" VERSION 1.0)

# Use C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

include(FetchContent)
FetchContent_Declare(
    Tau
    URL https://github.com/jasmcaus/tau/archive/dev.zip
)

FetchContent_MakeAvailable(Tau)

file(GLOB ALL_SOURCES "${WW_ROOT_PATH}/tweenylib/src/*.c" "${WW_ROOT_PATH}/runtime/src/*.c")
list(REMOVE_ITEM ALL_SOURCES "${WW_ROOT_PATH}/runtime/src/Main.c")

add_executable(WADsworthTests
    ${ALL_SOURCES}
    "TMain.c"
)

target_include_directories(WADsworthTests PUBLIC 
    "${WW_ROOT_PATH}/tweenylib/include" 
    "${WW_ROOT_PATH}/runtime/include"
)

target_link_libraries(WADsworthTests Tau)

if(MSVC)
    target_compile_options(WADsworthTests PRIVATE /W4 /WX)
else()
    target_compile_options(WADsworthTests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()