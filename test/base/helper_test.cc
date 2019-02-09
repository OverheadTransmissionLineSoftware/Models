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

TEST(Helper, DoubleToString) {
  std::string str;
  double value = 0;

  // tests entire number precision
  value = 1.23456789;
  str = helper::DoubleToString(value, 6);
  EXPECT_EQ("1.23457", str);

  value = 12345;
  str = helper::DoubleToString(value, 6);
  EXPECT_EQ("12345", str);

  // tests fixed decimal precision
  value = 1.23456789;
  str = helper::DoubleToString(value, 3, true);
  EXPECT_EQ("1.235", str);

  value = 12345;
  str = helper::DoubleToString(value, 3, true);
  EXPECT_EQ("12345.000", str);

  // scientific notation is not tested
  // exponent formatting is compiler-specific
}

TEST(Helper, IsNumeric) {
  std::string str;

  str = "-0.75";
  EXPECT_TRUE(helper::IsNumeric(str));

  str = str + "a";
  EXPECT_FALSE(helper::IsNumeric(str));
}

TEST(Helper, LinearY) {
  const double x1 = 0;
  const double y1 = 0;

  const double x2 = 10;
  const double y2 = 10;

  const double x = 5;
  const double y = helper::LinearY(x1, y1, x2, y2, x);
  EXPECT_EQ(5, helper::Round(y, 0));
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
