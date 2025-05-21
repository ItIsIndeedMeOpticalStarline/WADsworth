#include "Endianness.h"

TEST(Endianness, system_endianness)
{
    int32 i = 1;
    #ifdef WW_BIG_ENDIAN
    CHECK_NE(*(int8*)&i, 1);
    #else
    CHECK_EQ(*(int8*)&i, 1);
    #endif
}

TEST(Endianness, swap_int32)
{
    const int32 little = 0xABCD0000;
    const int32 big = 0x000CDAB;

    CHECK_EQ(SwapInt32(little), big);
    CHECK_EQ(SwapInt32(big), little);
}

// This is to check that the correct macros are defined, not that SwapInt32 works.
TEST(Endianness, htwi_with_macros)
{
    const int32 i = 1;
    if (*(int8*)&i == 1) // Little endian
    {
        CHECK_EQ(HostToWadInt(i), i);
        CHECK_EQ(WadIntToHost(i), i);
    }
    else // Big endian
    {
        CHECK_NE(HostToWadInt(i), i);
        CHECK_NE(WadIntToHost(i), i);
    }
}