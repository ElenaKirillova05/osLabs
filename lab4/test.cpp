#include <gtest/gtest.h>
#include "include/gcd_euclid_and_binary.h"
#include "include/gcd_naive_and_ternary.h"
#include <dlfcn.h>

typedef int (*GCD_Function)(int, int);
typedef char *(*Translation_Function)(long);

class GCDTest : public ::testing::Test {
protected:
    void *lib_handle;
    GCD_Function GCD;
    Translation_Function translation;

    void LoadLibrary(const char *lib_path) {
        lib_handle = dlopen(lib_path, RTLD_LAZY);
        ASSERT_NE(lib_handle, nullptr) << "Ошибка загрузки библиотеки: " << dlerror();

        GCD = (GCD_Function)dlsym(lib_handle, "GCD");
        translation = (Translation_Function)dlsym(lib_handle, "translation");
        ASSERT_NE(GCD, nullptr) << "Ошибка загрузки функции GCD: " << dlerror();
        ASSERT_NE(translation, nullptr) << "Ошибка загрузки функции translation: " << dlerror();
    }

    void UnloadLibrary() {
        if (lib_handle) dlclose(lib_handle);
    }

    void SetUp() override {
        LoadLibrary("./libgcd_euclid_and_binary.so");
    }

    void TearDown() override {
        UnloadLibrary();
    }
};

TEST_F(GCDTest, GCD_Euclid_BasicCases) {
    EXPECT_EQ(GCD(48, 18), 6);
    EXPECT_EQ(GCD(56, 98), 14);
    EXPECT_EQ(GCD(101, 103), 1);
    EXPECT_EQ(GCD(36, 60), 12);
}

TEST_F(GCDTest, BinaryTranslation_BasicCases) {
    EXPECT_STREQ(translation(10), "1010");
    EXPECT_STREQ(translation(255), "11111111");
    EXPECT_STREQ(translation(1), "1");
}


TEST_F(GCDTest, SwitchToNaiveAndTest) {
    UnloadLibrary();
    LoadLibrary("./libgcd_naive_and_ternary.so");

    EXPECT_EQ(GCD(48, 18), 6);
    EXPECT_EQ(GCD(56, 98), 14);
    EXPECT_EQ(GCD(101, 103), 1);
    EXPECT_EQ(GCD(36, 60), 12);

    EXPECT_STREQ(translation(10), "101");
    EXPECT_STREQ(translation(255), "100110");
    EXPECT_STREQ(translation(1), "1");
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
