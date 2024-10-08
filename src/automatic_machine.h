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

struct NormolizedAutomaticMachine;

struct Vertex {
  Vertex(size_t& id_vertex, bool finished, size_t vec_ind,
         const std::vector<std::pair<Vertex*, std::string>>& vec_edge) :
      id(id_vertex++), finished(finished), vec_ind(vec_ind), vec_edge(vec_edge) {}

  Vertex(size_t& id_vertex, bool finished, size_t vec_ind) :
      id(id_vertex++), finished(finished), vec_ind(vec_ind) {}

  size_t id;
  bool finished;
  size_t vec_ind;
  std::vector<std::pair<Vertex*, std::string>> vec_edge;
};

struct AutomaticMachine {
  AutomaticMachine() : root_vertex(nullptr) {}

  AutomaticMachine(Vertex* root_vertex) : root_vertex(root_vertex) {}

  AutomaticMachine(char ch) {
    auto* start_vertex = new Vertex{id_vertex, false, {0}};
    auto* finish_vertex = new Vertex{id_vertex, true, {1}};
    if (ch == '0') {
      start_vertex->vec_edge.emplace_back(finish_vertex, "!"); // ! - no transition
    } else if (ch == '1') {
      start_vertex->vec_edge.emplace_back(finish_vertex, "@");// @ - eps
    } else {
      start_vertex->vec_edge.emplace_back(finish_vertex, std::string(1, ch));
    }
    root_vertex = start_vertex;
  }

  AutomaticMachine(const std::string& str) {   // regular expression to NKA
    std::vector<AutomaticMachine> stack_token;
    std::vector<char> stack_operator;
    for (auto&& it : str) {
      if (map_priority.contains(it)) {
        StackFolding(stack_token, stack_operator, it);
      } else {
        stack_token.emplace_back(it);
      }
    }
    StackFinalFolding(stack_token, stack_operator);
    *this = stack_token.back();
  }

  AutomaticMachine(const AutomaticMachine& other) : root_vertex(other.root_vertex) {
    auto& a = const_cast<AutomaticMachine&>(other);
    a.root_vertex = nullptr;
  }

  AutomaticMachine& operator=(const AutomaticMachine& other) {
    if (root_vertex != other.root_vertex) {
      root_vertex = other.root_vertex;
    }
    auto& a = const_cast<AutomaticMachine&>(other);
    a.root_vertex = nullptr;
    return *this;
  }

  ~AutomaticMachine() {
    if (root_vertex != nullptr) {
      std::vector<bool> used(id_vertex + 1, false);
      std::vector<Vertex*> RemoveVector;
      DfsRemove(root_vertex, used, RemoveVector);
      for (auto&& it : RemoveVector) {
        delete it;
      }
    }
  }

  static AutomaticMachine CreateAutomaticMachine() {
    size_t count_vertex;
    std::cin >> count_vertex;
    std::vector<Vertex*> vec_vertex(count_vertex);
    for (size_t i = 0; i < count_vertex; ++i) {
      vec_vertex[i] = new Vertex(id_vertex, false, i);
    }
    for (size_t i = 0; i < count_vertex; ++i) {
      bool vertex_finished;
      std::cin >> vertex_finished;
      vec_vertex[i]->finished = vertex_finished;
      size_t count_edge;
      std::cin >> count_edge;
      for (size_t j = 0; j < count_edge; ++j) {
        size_t other_vertex;
        std::string str;
        std::cin >> other_vertex >> str;
        --other_vertex;
        vec_vertex[i]->vec_edge.emplace_back(vec_vertex[other_vertex], str);
      }
    }
    return vec_vertex[0];
  }

  void Copied(AutomaticMachine& other) const;

  static NormolizedAutomaticMachine RemoveEps(
      std::vector<std::vector<std::pair<size_t, std::string>>>& graph,
  std::vector<bool>& vec_finished);

  NormolizedAutomaticMachine RemoveEps();

  void Presentation();

  std::string CreateRegularExpByNKA();

  bool DfsOnString(size_t vertex, std::string str);


 private:

  void CopiedHelper(Vertex* vertex, Vertex* other_vertex,
                    AutomaticMachine& other, std::vector<bool>& vec_used,
                    std::map<Vertex*, Vertex*>& map_mapping) const;

  void DfsRemove(Vertex* vertex, std::vector<bool>& used, std::vector<Vertex*>& delete_vec);

  struct EdgeHelp {
    size_t id;
    size_t this_ver;
    size_t to_ver;
    std::string str;
  };

  static void DfsRemoveEpsTrnslation(
      std::vector<std::vector<std::pair<size_t, std::string>>>& vec_graph,
  std::vector<bool>& vec_finished);

  void HangingGraphToNormalGraph(
      std::vector<std::vector<std::pair<size_t, std::string>>>& graph,
  std::vector<std::vector<std::pair<size_t, std::string>>>& graph_reverse,
  std::vector<bool>& vec_finished,
      std::vector<size_t>& vec_ind);

  void HangingGraphToNormalGraphHelper(
      Vertex* vertex,
      std::vector<std::vector<std::pair<size_t, std::string>>>& graph,
  std::vector<std::vector<std::pair<size_t, std::string>>>& graph_reverse,
  std::vector<bool>& vec_finished,
      std::vector<size_t>& vec_ind,
  std::vector<bool>& used);

  static void TopSort(std::vector<bool>& used,
                      std::vector<std::vector<std::pair<size_t, std::string>>

  >& graph,
  size_t vertex, std::vector<size_t>
  & top_sort);

  static void GraphCondensation(std::vector<size_t>& ans,
                                std::vector<std::vector<std::pair<size_t, std::string>>

  >& graph_reverse,
  size_t vertex, size_t
  & comp);

  void DfsReordering(Vertex* root);

  void DfsReorderingHelper(Vertex* vertex, size_t& counter, std::vector<bool>& used);

  AutomaticMachine PlusAutomaticMachine(AutomaticMachine& first, AutomaticMachine& second);

  AutomaticMachine MultiplicationAutomaticMachine(AutomaticMachine& first,
                                                  AutomaticMachine& second);

  void DfsHelperToMultiplicationAutomaticMachineHelper(Vertex* vertex,
                                                       Vertex* start_vertex_other,
                                                       std::vector<bool>& used);

  AutomaticMachine StarKliniAutomaticMachine(AutomaticMachine& first);

  AutomaticMachine PlusKliniAutomaticMachine(AutomaticMachine& first);

  void DfsHelperToPlusKliniAutomaticMachineHelper(Vertex* vertex,
                                                  Vertex* start_vertex_other,
                                                  std::vector<bool>& used);

  void Folding(std::vector<AutomaticMachine>& stack_token,
               std::vector<char>& stack_operator);

  void StackFolding(std::vector<AutomaticMachine>& stack_token,
                    std::vector<char>& stack_operator, char last_operator);

  void StackFinalFolding(std::vector<AutomaticMachine>& stack_token,
                         std::vector<char>& stack_operator);

  void PresentationHelper(Vertex* vertex, std::vector<bool>& used);

  inline static std::map<char, size_t> map_priority =
      {{'+', 0},
       {'$', 1},
       {'*', 2},
       {'^', 2},
       {'(', 3},
       {')', 3}}; // ^ - plus Klini, $ - multiplication

  Vertex* root_vertex;
  inline static size_t id_vertex = 0;
};