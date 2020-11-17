#include "print_ip.h"
#include <list>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(print_ip, on_char)
{
ASSERT_EQ("255", print_ip(char(-1)));
ASSERT_EQ("255", print_ip(char(0xFF)));
ASSERT_EQ("31", print_ip(char(0x1F)));
}

TEST(print_ip, on_short)
{
ASSERT_EQ("0.0", print_ip(short(0)));
ASSERT_EQ("255.255", print_ip(short(0xFFFF)));
ASSERT_EQ("255.255", print_ip(short(-1)));
ASSERT_EQ("240.15", print_ip(short(0xF00F)));
}

TEST(print_ip, on_int)
{
ASSERT_EQ("127.0.0.1", print_ip(int(2130706433)));
ASSERT_EQ("0.0.0.0", print_ip(int(0)));
ASSERT_EQ("255.255.255.255", print_ip(int(-1)));
}

TEST(print_ip, on_long)
{
ASSERT_EQ("123.45.67.89.101.112.131.41", print_ip(long(8875824491850138409)));
ASSERT_EQ("0.0.0.0.0.0.0.0", print_ip(long(0)));
ASSERT_EQ("255.255.255.255.255.255.255.255", print_ip(long(-1)));
}

TEST(print_ip, on_string)
{
ASSERT_EQ("AB.CD.EF.GH", print_ip("AB.CD.EF.GH"));
}

TEST(print_ip, on_vector)
{
ASSERT_EQ("1.2.3.4.5", print_ip(std::vector<int>{1,2,3,4,5}));
ASSERT_EQ("10000.1", print_ip(std::vector<int>{10000,1}));
}

TEST(print_ip, on_list)
{
ASSERT_EQ("1.2.3.4.5", print_ip(std::list<int>{1,2,3,4,5}));
ASSERT_EQ("10000.1", print_ip(std::list<int>{10000,1}));
}

TEST(print_ip, on_tuple)
{
ASSERT_EQ("1.2.3.4.5", print_ip(std::make_tuple(1, 2, 3, 4, 5)));
ASSERT_EQ("5.4.A.B", print_ip(std::make_tuple("5", "4", "A", "B")));
}