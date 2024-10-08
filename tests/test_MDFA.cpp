#include <gtest/gtest.h>
#include "src/normolized_automatic_machine.h"


TEST (MDFA, CreateMDFA1) {
  NormolizedAutomaticMachine b("b+(b$(a$a)*$(a+b)*$a$a$b*)$(1+b$a$b*)");
  b.BuildPDKA({"a", "b"});
  b.BuildMinPDKAByPDKA();
  EXPECT_EQ(b.GetCountVertex(), 9);
}

TEST (MDFA, CreateMDFA2) {
  NormolizedAutomaticMachine b("(b+a$a$(b$a)*$a)^");
  b.BuildPDKA({"a", "b"});
  b.BuildMinPDKAByPDKA();
  EXPECT_EQ(b.GetCountVertex(), 5);
}

TEST (MDFA, CreateMDFA3) {
  NormolizedAutomaticMachine b("(((1+1)*)^+(1*$1^)*)*");
  b.BuildPDKA({"a", "b"});
  b.BuildMinPDKAByPDKA();
  EXPECT_EQ(b.GetCountVertex(), 2);
}