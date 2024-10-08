#include <gtest/gtest.h>
#include "src/automatic_machine.h"
#include <fstream>


TEST (NFA, CreateNFAByString1) {
  std::streambuf* cinbuf = std::cin.rdbuf();
  std::ifstream in("../tests/input_test_create_to_string1.txt");
  std::cin.rdbuf(in.rdbuf());
  AutomaticMachine a = AutomaticMachine::CreateAutomaticMachine();
  EXPECT_TRUE(a.DfsOnString(0, "abba"));
  EXPECT_TRUE(a.DfsOnString(0, "abbbbbbbb"));
  EXPECT_TRUE(a.DfsOnString(0, "abbbbbaaaaaaaaaa"));
  EXPECT_TRUE(a.DfsOnString(0, ""));
  EXPECT_FALSE(a.DfsOnString(0, "baaaa"));
  EXPECT_FALSE(a.DfsOnString(0, "abab"));
  EXPECT_FALSE(a.DfsOnString(0, "abcab"));
  std::cin.rdbuf(cinbuf);
}

TEST (NFA, CreateNFAByString2) {
  std::streambuf* cinbuf = std::cin.rdbuf();
  std::ifstream in("../tests/input_test_create_to_string2.txt");
  std::cin.rdbuf(in.rdbuf());
  AutomaticMachine a = AutomaticMachine::CreateAutomaticMachine();
  EXPECT_TRUE(a.DfsOnString(0, ""));
  EXPECT_TRUE(a.DfsOnString(0, "ab"));
  EXPECT_TRUE(a.DfsOnString(0, "abaababb"));
  EXPECT_TRUE(a.DfsOnString(0, "abababa"));
  EXPECT_TRUE(a.DfsOnString(0, "abababb"));
  EXPECT_FALSE(a.DfsOnString(0, "b"));
  EXPECT_FALSE(a.DfsOnString(0, "bbba"));
  EXPECT_FALSE(a.DfsOnString(0, "abaaab"));
  EXPECT_FALSE(a.DfsOnString(0, "abaabbb"));
  std::cin.rdbuf(cinbuf);
}