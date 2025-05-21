cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Use C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

set(WW_SOURCES 
    "src/Main.c"
)
set(WW_INCLUDES "${WW_ROOT_PATH}/runtime/include" "${WW_ROOT_PATH}/tweenylib/include")
set(WW_LIBRARIES SDL3::SDL3)

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(WW_SOURCES ${WW_SOURCES} ${TWEENYLIB_SOURCES})
    set(WW_LIBRARIES ${WW_SOURCES} TweenyLib)
endif()

add_executable(WADsworth ${WW_SOURCES})

target_include_directories(WADsworth PUBLIC ${WW_INCLUDES})

target_link_libraries(WADsworth PRIVATE ${WW_LIBRARIES})

if(MSVC)
    target_compile_options(WADsworth PRIVATE /W4 /WX)
else()
    target_compile_options(WADsworth PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()