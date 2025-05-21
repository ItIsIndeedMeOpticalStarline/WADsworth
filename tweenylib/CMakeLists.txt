cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Use C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

set(TWEENYLIB_SOURCES
    "src/Algorithms.c"
    "src/Endianness.c"
    "src/Errors.c"
    "src/Files.c"
    "src/Memory.c"
    "src/WAD.c"
    "src/WADStr.c"
    "src/WWLocale.c"
    "src/WWMath.c"
)

set(TWEENYLIB_INCLUDES "${WW_ROOT_PATH}/tweenylib/include")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_library(TweenyLib SHARED ${TWEENYLIB_SOURCES})

    target_include_directories(TweenyLib PUBLIC ${TWEENYLIB_INCLUDES})

    if(MSVC)
        target_compile_options(TweenyLib PRIVATE /W4 /WX)
    else()
        target_compile_options(TweenyLib PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endif()