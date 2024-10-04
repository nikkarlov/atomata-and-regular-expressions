#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include "normolized_automatic_machine.h"
#include "automatic_machine.h"

int main() {
  //std::freopen("input.txt", "r", stdin);
  //std::freopen("output.txt", "w", stdout);
  std::string str;
  std::cin >> str;
  AutomaticMachine a(str);
  //a.Presentation();
  auto b = a.RemoveEps();
  b.DeleteTraslationWithMoreOneWord();
  b.AlgoTompson();
  b.BuildPDKA({"a", "b"});
  b.BuildMinPDKAByPDKA();
  std::cout << b.CreateRegularExpByDKA();
  //b.Presintation();
  //std::cout << b.DfsOnString(0, "aabbbabaa");
  return 0;
}
