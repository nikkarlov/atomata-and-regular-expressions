#include <gtest/gtest.h>
#include "src/normolized_automatic_machine.h"
#include "src/automatic_machine.h"


TEST (CreateByRegExp, CreateNFAByRegExp) {
  AutomaticMachine a("a$b+(1+a)*");
  EXPECT_TRUE(a.DfsOnString(0, "ab"));
  EXPECT_TRUE(a.DfsOnString(0, ""));
  EXPECT_TRUE(a.DfsOnString(0, "aaaaaaaa"));
  EXPECT_FALSE(a.DfsOnString(0, "aba"));
  EXPECT_FALSE(a.DfsOnString(0, "abaaaaaa"));
  EXPECT_FALSE(a.DfsOnString(0, "abab"));
  EXPECT_FALSE(a.DfsOnString(0, "abb"));
}

TEST (CreateByRegExp, CreateDFAByRegExp) {
  AutomaticMachine a("(b+a$a$(b$a)*$a)^");
  EXPECT_TRUE(a.DfsOnString(0, "b"));
  EXPECT_TRUE(a.DfsOnString(0, "bbbbb"));
  EXPECT_TRUE(a.DfsOnString(0, "baaaaababaa"));
  EXPECT_TRUE(a.DfsOnString(0, "aabaabaaa"));
  EXPECT_FALSE(a.DfsOnString(0, ""));
  EXPECT_FALSE(a.DfsOnString(0, "bab"));
  EXPECT_FALSE(a.DfsOnString(0, "aab"));
  EXPECT_FALSE(a.DfsOnString(0, "baabaaa"));
}