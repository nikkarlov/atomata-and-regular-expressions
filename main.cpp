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
#include "src/normolized_automatic_machine.cpp"
#include "src/automatic_machine.cpp"

int main() {
  //std::freopen("input.txt", "r", stdin);
  //std::freopen("output.txt", "w", stdout);
  std::string str;
  std::cin >> str;
  AutomaticMachine a(str);
  std::cout<<a.DfsOnString(0, "ab");
  //a.Presentation();
  auto b = a.RemoveEps();
  b.DeleteTraslationWithMoreOneWord();
  b.AlgoTompson();
  b.BuildPDKA({"a", "b"});
  b.BuildMinPDKAByPDKA();
  //std::cout<<b.DfsOnString(0, "ab");
  //std::cout << b.CreateRegularExpByDKA();
  //b.Presintation();
  //std::cout << b.DfsOnString(0, "b");
  return 0;
}