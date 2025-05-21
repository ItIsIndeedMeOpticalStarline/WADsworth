#include "WWMath.h"

TEST(Math, max_macro)
{
    CHECK_EQ(Max(0u, 0u), 0u);
    CHECK_EQ(Max(12, 10), 12);
    CHECK_EQ(Max(0.0f, 0.045f), 0.045f);

    CHECK_EQ(Max(-12, 10), 10);
    CHECK_EQ(Max(0.0f, -0.045f), 0.0f);
}

TEST(Math, min_macro)
{
    CHECK_EQ(Min(0u, 0u), 0u);
    CHECK_EQ(Min(12, 10), 10);
    CHECK_EQ(Min(0.0f, 0.045f), 0.0f);

    CHECK_EQ(Min(-12, 10), -12);
    CHECK_EQ(Min(0.0f, -0.045f), -0.045f);
}

TEST(Math, clamp_macro)
{
    CHECK_EQ(Clamp(1000u, 0u, 255u), 255u);
    CHECK_EQ(Clamp(-1000, 20, 255), 20);
    CHECK_EQ(Clamp(1000.0f, 1.0f, 1000000.0f), 1000.0f);

    CHECK_EQ(Clamp(-1000, -20, 255), -20);
    CHECK_EQ(Clamp(-1000.0f, -1.0f, 1000000.0f), -1.0f);
}