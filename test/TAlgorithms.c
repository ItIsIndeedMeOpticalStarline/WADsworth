#include "Algorithms.h"

TEST(HashFunctions, djb2) 
{
    char* string1 = "The quick brown fox jumps over the lazy dog";
    char* string2 = "Any duel can go badly -50/50 Ghandi Proverb 01";
    char* string3 = "Araby died tonight, Struck down by a meteorite";

    uint32 result1 = DJB2((byte*)string1);
    uint32 result2 = DJB2((byte*)string2);
    uint32 result3 = DJB2((byte*)string3);

    CHECK_EQ(result1, 0x34CC38DE);
    CHECK_EQ(result2, 0x7E03EDFD);
    CHECK_EQ(result3, 0x9D06FFF1);
}