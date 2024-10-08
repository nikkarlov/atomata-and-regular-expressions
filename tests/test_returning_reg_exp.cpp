#include <gtest/gtest.h>
#include "src/normolized_automatic_machine.h"
#include "src/automatic_machine.h"

TEST (GetRegExp, GetDFARegExp) {
  NormolizedAutomaticMachine a("a$b+(1+a)*");
  NormolizedAutomaticMachine b(a.CreateRegularExpByDKA());
  EXPECT_TRUE(b.DfsOnString(0, "ab"));
  EXPECT_TRUE(b.DfsOnString(0, ""));
  EXPECT_TRUE(b.DfsOnString(0, "aaaaaaaa"));
  EXPECT_FALSE(b.DfsOnString(0, "aba"));
  EXPECT_FALSE(b.DfsOnString(0, "abaaaaaa"));
  EXPECT_FALSE(b.DfsOnString(0, "abab"));
  EXPECT_FALSE(b.DfsOnString(0, "abb"));
}

TEST (GetRegExp, GetNFARegExp) {
  AutomaticMachine a("(b+a$a$(b$a)*$a)^");
  AutomaticMachine b(a.CreateRegularExpByNKA());
  EXPECT_TRUE(b.DfsOnString(0, "b"));
  EXPECT_TRUE(b.DfsOnString(0, "bbbbb"));
  EXPECT_TRUE(b.DfsOnString(0, "baaaaababaa"));
  EXPECT_TRUE(b.DfsOnString(0, "aabaabaaa"));
  EXPECT_FALSE(b.DfsOnString(0, ""));
  EXPECT_FALSE(b.DfsOnString(0, "bab"));
  EXPECT_FALSE(b.DfsOnString(0, "aab"));
  EXPECT_FALSE(b.DfsOnString(0, "baabaaa"));
}