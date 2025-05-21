cmake_minimum_required(VERSION 3.20)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("WADsworth" VERSION 1.0)
set(WW_ROOT_PATH ${PROJECT_SOURCE_DIR})

# Use C17 standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED true)

# Make a define if we are big endian
if (CMAKE_VERSION VERSION_LESS 3.20)
    include (TestBigEndian)
    test_big_endian(IS_BIG_ENDIAN)
    if (IS_BIG_ENDIAN)
        add_compile_definitions(WW_BIG_ENDIAN)
    endif()
else()
    if (CMAKE_C_BYTE_ORDER STREQUAL "BIG_ENDIAN")
        add_compile_definitions(WW_BIG_ENDIAN)
    endif()
endif()

# External library
add_subdirectory("tweenylib")

# SDL3
add_subdirectory("${WW_ROOT_PATH}/vendored/SDL" EXCLUDE_FROM_ALL)

# Runtime application
add_subdirectory("runtime")

set(WW_BUILDTESTS TRUE)

if (WW_BUILDTESTS)
    # Unit tests
    add_subdirectory("test")
endif()