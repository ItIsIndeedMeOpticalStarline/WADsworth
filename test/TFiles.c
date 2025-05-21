#include "Files.h"

TEST(Paths, new_path)
{
    const char* relativePathCStr = "./This/is/a relative/path thing";
    Path relativePath = { 0 };
    ww_ok ok = New_Path(&relativePath, relativePathCStr);
    CHECK_EQ((int)ok, (int)true);
    CHECK_EQ((int)relativePath.absolute, (int)false);
    CHECK_STREQ(relativePath.extension, "");
    CHECK_STREQ(relativePath.fileName, "");
    CHECK_STREQ(relativePath.pathToFile, "./This/is/a relative/path thing");

    const char* absolutePathCStr = "C:/This/is/an absolute/path thing";
    Path absolutePath = { 0 };
    ok = New_Path(&absolutePath, absolutePathCStr);
    CHECK_EQ((int)ok, (int)true);
    CHECK_EQ((int)absolutePath.absolute, (int)true);
    CHECK_STREQ(absolutePath.extension, "");
    CHECK_STREQ(absolutePath.fileName, "");
    CHECK_STREQ(absolutePath.pathToFile, "C:/This/is/an absolute/path thing");

    const char* relativeFilePathCStr = "./This/is/a relative/file path.";
    Path relativeFilePath = { 0 };
    ok = New_Path(&relativeFilePath, relativeFilePathCStr);
    CHECK_EQ((int)ok, (int)true);
    CHECK_EQ((int)relativeFilePath.absolute, (int)false);
    CHECK_STREQ(relativeFilePath.extension, ".");
    CHECK_STREQ(relativeFilePath.fileName, "file path");
    CHECK_STREQ(relativeFilePath.pathToFile, "./This/is/a relative/");

    const char* absoluteFilePathCStr = "C:/This/is/an/absolute/file path.abcdefghijkl mnopqrstuvwxyz";
    Path absoluteFilePath = { 0 };
    ok = New_Path(&absoluteFilePath, absoluteFilePathCStr);
    CHECK_EQ((int)ok, (int)true);
    CHECK_EQ((int)absoluteFilePath.absolute, (int)true);
    CHECK_STREQ(absoluteFilePath.extension, ".abcdefghijkl mnopqrstuvwxyz");
    CHECK_STREQ(absoluteFilePath.fileName, "file path");
    CHECK_STREQ(absoluteFilePath.pathToFile, "C:/This/is/an/absolute/");

    free(relativePath.extension);
    free(relativePath.fileName);
    free(relativePath.pathToFile);

    free(absolutePath.extension);
    free(absolutePath.fileName);
    free(absolutePath.pathToFile);

    free(relativeFilePath.extension);
    free(relativeFilePath.fileName);
    free(relativeFilePath.pathToFile);

    free(absoluteFilePath.extension);
    free(absoluteFilePath.fileName);
    free(absoluteFilePath.pathToFile);
}

TEST(Paths, to_cstr_path)
{
    Path path =
    {
        false,
        malloc(sizeof("")),
        malloc(sizeof("")),
        malloc(sizeof("./Boy friend/Mother's")),
    };
    *path.extension = '\0';
    *path.fileName = '\0';
    strncpy_s(path.pathToFile, sizeof("./Boy friend/Mother's"), "./Boy friend/Mother's", sizeof("./Boy friend/Mother's"));

    ww_char* pathCStr = NULL;
    ww_ok ok = ToCStr_Path(&pathCStr, &path);
    CHECK_EQ((int)ok, (int)true);
    CHECK_STREQ(pathCStr, "./Boy friend/Mother's");

    Path filePath =
    {
        true,
        malloc(sizeof(".Hello")),
        malloc(sizeof("your I'm")),
        malloc(sizeof("C:/Girl friend/Mother's/")),
    };
    strncpy_s(filePath.extension, sizeof(".Hello"), ".Hello", sizeof(".Hello"));
    strncpy_s(filePath.fileName, sizeof("your I'm"), "your I'm", sizeof("your I'm"));
    strncpy_s(filePath.pathToFile, sizeof("C:/Girl friend/Mother's/"), "C:/Girl friend/Mother's/", sizeof("C:/Girl friend/Mother's/"));

    ww_char* filePathCStr = NULL;
    ok = ToCStr_Path(&filePathCStr, &filePath);
    CHECK_EQ((int)ok, (int)true);
    CHECK_STREQ(filePathCStr, "C:/Girl friend/Mother's/your I'm.Hello");

    free(path.extension);
    free(path.fileName);
    free(path.pathToFile);

    free(pathCStr);

    free(filePath.extension);
    free(filePath.fileName);
    free(filePath.pathToFile);

    free(filePathCStr);
}

TEST(Paths, try_open_file_from_path)
{
    const char* testFilePathCStr = "./open_file_from_path__test.txt";
    FILE* dummy = fopen(testFilePathCStr, "wb");
    CHECK_NE((void*)dummy, NULL);
    CHECK_EQ(fclose(dummy), 0);

    Path filePath =
    {
        false,
        malloc(sizeof(".txt")),
        malloc(sizeof("open_file_from_path__test")),
        malloc(sizeof("./")),
    };
    strncpy_s(filePath.extension, sizeof(".txt"), ".txt", sizeof(".txt"));
    strncpy_s(filePath.fileName, sizeof("open_file_from_path__test"), "open_file_from_path__test", sizeof("open_file_from_path__test"));
    strncpy_s(filePath.pathToFile, sizeof("./"), "./", sizeof("./"));

    FILE* file = NULL;
    CHECK_EQ((int)TryOpenFileFromPath(&file, &filePath, "rb"), (int)true);
    fclose(file);

    free(filePath.extension);
    free(filePath.fileName);
    free(filePath.pathToFile);

    remove(testFilePathCStr);
}

TEST(Paths, try_open_file_from_cstr)
{
    const char* testFilePathCStr = "./try_open_file_from_cstr__test.txt";
    FILE* dummy = fopen(testFilePathCStr, "wb");
    CHECK_NE((void*)dummy, NULL);
    CHECK_EQ(fclose(dummy), 0);

    FILE* file = NULL;
    CHECK_EQ((int)TryOpenFileFromCStr(&file, testFilePathCStr, "rb"), (int)true);
    fclose(file);

    remove(testFilePathCStr);
}