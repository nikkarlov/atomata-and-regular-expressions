#pragma once

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
#include "automatic_machine.h"

struct NormolizedAutomaticMachine {
  NormolizedAutomaticMachine(
      const std::vector<std::vector<std::pair<size_t, std::string>>>& graph,
      const std::vector<bool>& vec_finished) :
      graph(graph),
      vec_finished(vec_finished) {}

  NormolizedAutomaticMachine(std::string reg_exp) {
    AutomaticMachine a(reg_exp);
    auto b = a.RemoveEps();
    b.DeleteTraslationWithMoreOneWord();
    b.AlgoTompson();
    std::swap(graph, b.graph);
    std::swap(vec_finished, b.vec_finished);
  }

  NormolizedAutomaticMachine(const AutomaticMachine& a) {
    AutomaticMachine a_copy;
    a.Copied(a_copy);
    auto b = a_copy.RemoveEps();
    b.DeleteTraslationWithMoreOneWord();
    b.AlgoTompson();
    std::swap(graph, b.graph);
    std::swap(vec_finished, b.vec_finished);
  }

  static NormolizedAutomaticMachine CreateNormolizedAutomaticMachine() {
    size_t count_vertex;
    std::cin >> count_vertex;
    std::vector<std::vector<std::pair<size_t, std::string>>> vec_graph(count_vertex);
    std::vector<bool> vec_finished(count_vertex, false);
    for (size_t i = 0; i < count_vertex; ++i) {
      bool vertex_finished;
      std::cin >> vertex_finished;
      vec_finished[i] = vertex_finished;
      size_t count_edge;
      std::cin >> count_edge;
      for (size_t j = 0; j < count_edge; ++j) {
        size_t other_vertex;
        std::string str;
        std::cin >> other_vertex >> str;
        --other_vertex;
        vec_graph[i].emplace_back(other_vertex, str);
      }
    }
    return {vec_graph, vec_finished};
  }

  bool DfsOnString(size_t vertex, std::string str);


  void Presintation();

  void DeleteTraslationWithMoreOneWord();

  void AlgoTompson();

  void BuildPDKA(const std::vector<std::string>& vec_alfabet);

  void SupplementPDKA();

  void BuildMinPDKAByPDKA();

  size_t GetCountVertex();

  std::string CreateRegularExpByDKA();

 private:
  std::vector<std::vector<std::pair<size_t, std::string>>> graph;
  std::vector<bool> vec_finished;
};