// This is free and unencumbered software released into the public domain.
// For more information, please refer to <http://unlicense.org/>

#include "models/base/helper.h"

#include "gtest/gtest.h"

TEST(Helper, CompareStrings) {
  std::string str1;
  std::string str2;

  // tests equality
  str1 = "Test";
  str2 = "Test";
  EXPECT_EQ(0, helper::CompareStrings(str1, str2));

  // tests character comparison
  str1 = "a";
  str2 = "b";
  EXPECT_EQ(-1, helper::CompareStrings(str1, str2));

  // tests length
  str1 = "11111";
  str2 = "1111";
  EXPECT_EQ(1, helper::CompareStrings(str1, str2));
}

TEST(Helper, IsNumeric) {
  std::string str;

  str = "-0.75";
  EXPECT_TRUE(helper::IsNumeric(str));

  str = str + "a";
  EXPECT_FALSE(helper::IsNumeric(str));
}

TEST(Helper, Parse) {
  std::string str = "0,1,2,3";

  std::list<std::string> strs = helper::Parse(str, ',');
  std::string str_parsed;

  str_parsed = *std::next(strs.cbegin(), 0);
  EXPECT_EQ("0", str_parsed);

  str_parsed = *std::next(strs.cbegin(), 1);
  EXPECT_EQ("1", str_parsed);

  str_parsed = *std::next(strs.cbegin(), 2);
  EXPECT_EQ("2", str_parsed);

  str_parsed = *std::next(strs.cbegin(), 3);
  EXPECT_EQ("3", str_parsed);
}

TEST(Helper, Round) {
  const double value = 123.45;

  EXPECT_EQ(123, helper::Round(value, 0));
  EXPECT_EQ(123.5, helper::Round(value, 1));
  EXPECT_EQ(123.45, helper::Round(value, 2));
  EXPECT_EQ(123.450, helper::Round(value, 3));
}
