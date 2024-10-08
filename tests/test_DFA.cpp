#include <gtest/gtest.h>
#include "src/automatic_machine.h"
#include "src/normolized_automatic_machine.h"
#include <fstream>


TEST (DFA, CreateDFAByNFA) {
  std::streambuf* cinbuf = std::cin.rdbuf();
  std::ifstream in("../tests/input_test_create_to_string1.txt");
  std::cin.rdbuf(in.rdbuf());
  AutomaticMachine a = AutomaticMachine::CreateAutomaticMachine();
  NormolizedAutomaticMachine b(a);
  EXPECT_TRUE(b.DfsOnString(0, "abba"));
  EXPECT_TRUE(b.DfsOnString(0, "abbbbbbbb"));
  EXPECT_TRUE(b.DfsOnString(0, "abbbbbaaaaaaaaaa"));
  EXPECT_TRUE(b.DfsOnString(0, ""));
  EXPECT_FALSE(b.DfsOnString(0, "baaaa"));
  EXPECT_FALSE(b.DfsOnString(0, "abab"));
  EXPECT_FALSE(b.DfsOnString(0, "abcab"));
  std::cin.rdbuf(cinbuf);
}

TEST (DFA, CreateDFAByString) {
  std::streambuf* cinbuf = std::cin.rdbuf();
  std::ifstream in("../tests/input_test_create_DFA_to_string.txt");
  std::cin.rdbuf(in.rdbuf());
  NormolizedAutomaticMachine a = NormolizedAutomaticMachine::CreateNormolizedAutomaticMachine();
  EXPECT_TRUE(a.DfsOnString(0, "a"));
  EXPECT_TRUE(a.DfsOnString(0, "bba"));
  EXPECT_TRUE(a.DfsOnString(0, "baaaaba"));
  EXPECT_TRUE(a.DfsOnString(0, "ab"));
  EXPECT_TRUE(a.DfsOnString(0, "bbb"));
  EXPECT_FALSE(a.DfsOnString(0, ""));
  EXPECT_FALSE(a.DfsOnString(0, "bbba"));
  EXPECT_FALSE(a.DfsOnString(0, "babb"));
  EXPECT_FALSE(a.DfsOnString(0, "aaa"));
  std::cin.rdbuf(cinbuf);
}
