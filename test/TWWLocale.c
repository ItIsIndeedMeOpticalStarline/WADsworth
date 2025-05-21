#include "WWLocale.h"

TEST(CString, latin_1_to_utf8)
{
    UTF8Me();

    uint8 latinString[256] = { 0 };

    for (size i = 0; i < 0xFF; i++)
        latinString[i] = (uint8)i + 1;

    ww_char* utf8String;
    ww_ok ok = Latin1ToUTF8(&utf8String, latinString);
    CHECK_EQ((int)ok, (int)1);

    const ww_char* const expectedUTF8String = u8"\x1\x2\x3\x4\x5\x6\x7\x8\x9\xA\xB\xC\xD\xE\xF"
                                              u8"\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F"
                                              u8" !\"#$%&'()*+,-./"
                                              u8"0123456789:;<=>?"
                                              u8"@ABCDEFGHIJKLMNO"
                                              u8"PQRSTUVWXYZ[\x5C]^_"
                                              u8"`abcdefghijklmno"
                                              u8"pqrstuvwxyz{|}~\x7F"
                                              u8"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F"
                                              u8"\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F"
                                              u8"\xA0¡¢£¤¥¦§¨©ª«¬\xAD®¯"
                                              u8"°±²³´µ¶·¸¹º»¼½¾¿"
                                              u8"ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏ"
                                              u8"ÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞß"
                                              u8"àáâãäåæçèéêëìíîï"
                                              u8"ðñòóôõö÷øùúûüýþÿ";

    CHECK_STREQ(utf8String, expectedUTF8String);
}

// No string comparison since UTF16ToUTF8 calls a proprietary windows function that should output
// valid UTF8. We just wanna make sure that UTF16ToUTF8 completes without failing
TEST(CString, utf16_to_utf8)
{
    UTF8Me();

    wchar_t utf16String[65536] = {0};

    for (size i = 0; i < 0xFFFF; i++)
        utf16String[i] = (wchar_t)i + 1;

    ww_char* utf8String;
    ww_ok ok = UTF16ToUTF8(&utf8String, utf16String);
    CHECK_EQ((int)ok, (int)1);
}