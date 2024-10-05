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

  static NormolizedAutomaticMachine RemoveEps(
      std::vector<std::vector<std::pair<size_t, std::string>>

      >& graph,
      std::vector<bool>& vec_finished
  ) {
    std::vector<std::vector<std::pair<size_t, std::string>>> graph_reverse(graph.size());
    for (size_t i = 0; i < graph.size(); ++i) {
      for (size_t j = 0; j < graph[i].size(); ++j) {
        graph_reverse[graph[i][j].first].emplace_back(i, graph[i][j].second);
      }
    }
    std::vector<bool> used(id_vertex + 1, false);
    std::vector<size_t> top_sort;
    for (size_t i = 0; i < id_vertex + 1; ++i) {
      if (!used[i]) {
        TopSort(used, graph, i, top_sort);
      }
    }
    std::vector<size_t> ans(id_vertex + 1, 0);
    size_t comp = 1;
    reverse(top_sort.begin(), top_sort.end());
    for (size_t i = 0; i < id_vertex + 1; ++i) {
      if (ans[top_sort[i]] == 0U) {
        GraphCondensation(ans, graph_reverse, top_sort[i], comp);
        ++comp;
      }
    }
    std::map<size_t, size_t> map_ind;
    std::vector<std::vector<std::pair<size_t, std::string>>> new_graph(graph.size());
    std::vector<bool> new_vec_finished(graph.size());
    for (size_t i = 0; i < graph.size(); ++i) {
      for (auto&& it : graph[i]) {
        new_graph[map_ind[ans[i]]].emplace_back(map_ind[ans[it.first]], it.second);
      }
      if (vec_finished[i]) {
        new_vec_finished[map_ind[ans[i]]] = true;
      }
    }
    DfsRemoveEpsTrnslation(new_graph, new_vec_finished);
    for (size_t i = 0; i < new_graph.size(); ++i) {
      std::set<std::pair<size_t, std::string>> st;
      for (long long j = 0; j < new_graph[i].size(); ++j) {
        if (st.contains(new_graph[i][j])) {
          std::swap(new_graph[i][j], new_graph[i].back());
          new_graph[i].pop_back();
          --j;
        }
        st.insert(new_graph[i][j]);
      }
    }
    return {new_graph, new_vec_finished};
  }

  NormolizedAutomaticMachine RemoveEps() { // NKA to NKA without epsilon transitions
    std::vector<std::vector<std::pair<size_t, std::string>>> graph(id_vertex + 1);
    std::vector<std::vector<std::pair<size_t, std::string>>> graph_reverse(id_vertex + 1);
    std::vector<bool> vec_finished(id_vertex + 1);
    std::vector<size_t> vec_ind;
    HangingGraphToNormalGraph(graph, graph_reverse, vec_finished, vec_ind);
    std::vector<bool> used(id_vertex + 1, false);
    std::vector<size_t> top_sort;
    for (size_t i = 0; i < id_vertex + 1; ++i) {
      if (!used[i]) {
        TopSort(used, graph, i, top_sort);
      }
    }
    std::vector<size_t> ans(id_vertex + 1, 0);
    size_t comp = 1;
    reverse(top_sort.begin(), top_sort.end());
    for (size_t i = 0; i < id_vertex + 1; ++i) {
      if (ans[top_sort[i]] == 0U) {
        GraphCondensation(ans, graph_reverse, top_sort[i], comp);
        ++comp;
      }
    }
    std::map<size_t, size_t> map_ind;
    size_t id_ind = 0;
    for (auto&& i : vec_ind) {
      if (!map_ind.contains(ans[i])) {
        map_ind[ans[i]] = id_ind++;
      }
    }
    std::vector<std::vector<std::pair<size_t, std::string>>> new_graph(id_ind);
    std::vector<bool> new_vec_finished(id_ind);
    for (auto&& i : vec_ind) {
      for (auto&& it : graph[i]) {
        new_graph[map_ind[ans[i]]].emplace_back(map_ind[ans[it.first]], it.second);
      }
      if (vec_finished[i]) {
        new_vec_finished[map_ind[ans[i]]] = true;
      }
    }
    DfsRemoveEpsTrnslation(new_graph, new_vec_finished);
    for (size_t i = 0; i < new_graph.size(); ++i) {
      std::set<std::pair<size_t, std::string>> st;
      for (long long j = 0; j < new_graph[i].size(); ++j) {
        if (st.contains(new_graph[i][j])) {
          std::swap(new_graph[i][j], new_graph[i].back());
          new_graph[i].pop_back();
          --j;
        }
        st.insert(new_graph[i][j]);
      }
    }
    return {new_graph, new_vec_finished};
  }

  void Presentation() {
    std::vector<bool> used(id_vertex + 1, false);
    PresentationHelper(root_vertex, used);
  }


 private:

  void DfsRemove(Vertex* vertex, std::vector<bool>& used, std::vector<Vertex*>& delete_vec) {
    used[vertex->id] = true;
    delete_vec.push_back(vertex);
    for (auto&& it : vertex->vec_edge) {
      if (!used[it.first->id]) {
        DfsRemove(it.first, used, delete_vec);
      }
    }
  }

  struct EdgeHelp {
    size_t id;
    size_t this_ver;
    size_t to_ver;
    std::string str;
  };

  static void DfsRemoveEpsTrnslation(
      std::vector<std::vector<std::pair<size_t, std::string>>

      >& vec_graph,
      std::vector<bool>& vec_finished
  ) {
    std::vector<std::set<std::pair<size_t, std::string>>> set_current_edge(vec_graph.size());
    std::vector<std::vector<EdgeHelp>> graph(vec_graph.size());
    std::vector<std::vector<EdgeHelp>> reverse_graph(vec_graph.size());
    std::map<size_t, bool> map_edge_delete;
    std::vector<EdgeHelp> vec_edge_eps;
    size_t id_edge = 0;
    for (size_t i = 0; i < vec_graph.size(); ++i) {
      for (size_t j = 0; j < vec_graph[i].size(); ++j) {
        set_current_edge[i].emplace(vec_graph[i][j].first, vec_graph[i][j].second);
        graph[i].emplace_back(++id_edge, i, vec_graph[i][j].first, vec_graph[i][j].second);
        reverse_graph[vec_graph[i][j].first].emplace_back(id_edge, vec_graph[i][j].first, i, vec_graph[i][j].second);
        map_edge_delete[id_edge] = false;
        if (vec_graph[i][j].second == "@") {
          vec_edge_eps.push_back(graph[i].back());
        }
      }
    }
    while (!vec_edge_eps.empty()) {
      auto edge_eps = vec_edge_eps.back();
      vec_edge_eps.pop_back();
      if (vec_finished[edge_eps.to_ver]) {
        vec_finished[edge_eps.this_ver] = true;
      }
      for (size_t i = 0; i < reverse_graph[edge_eps.this_ver].size(); ++i) {
        auto temp_edge = reverse_graph[edge_eps.this_ver][i];
        if (!map_edge_delete[temp_edge.id] &&
            !set_current_edge[temp_edge.to_ver].contains({edge_eps.to_ver, temp_edge.str})
            && !(temp_edge.to_ver == edge_eps.to_ver && temp_edge.str == "@")) {
          set_current_edge[temp_edge.to_ver].emplace(edge_eps.to_ver, temp_edge.str);
          graph[temp_edge.to_ver].emplace_back(
              ++id_edge, temp_edge.to_ver, edge_eps.to_ver, temp_edge.str);
          reverse_graph[edge_eps.to_ver].emplace_back(
              id_edge, edge_eps.to_ver, temp_edge.to_ver, temp_edge.str);
          map_edge_delete[id_edge] = false;
          if (temp_edge.str == "@") {
            vec_edge_eps.push_back(graph[temp_edge.to_ver].back());
          }
        }
      }
      for (size_t i = 0; i < graph[edge_eps.to_ver].size(); ++i) {
        auto temp_edge = graph[edge_eps.to_ver][i];
        if (!map_edge_delete[temp_edge.id] &&
            !set_current_edge[edge_eps.this_ver].contains({temp_edge.to_ver, temp_edge.str})
            && !(edge_eps.this_ver == temp_edge.to_ver && temp_edge.str == "@")) {
          set_current_edge[edge_eps.this_ver].emplace(temp_edge.to_ver, temp_edge.str);
          graph[edge_eps.this_ver].emplace_back(
              ++id_edge, edge_eps.this_ver, temp_edge.to_ver, temp_edge.str);
          reverse_graph[temp_edge.to_ver].emplace_back(
              id_edge, temp_edge.to_ver, edge_eps.this_ver, temp_edge.str);
          map_edge_delete[id_edge] = false;
          if (temp_edge.str == "@") {
            vec_edge_eps.push_back(graph[edge_eps.this_ver].back());
          }
        }
      }
      set_current_edge[edge_eps.this_ver].erase({edge_eps.to_ver, edge_eps.str});
      map_edge_delete[edge_eps.id] = true;
    }
    std::vector<std::vector<std::pair<size_t, std::string>>> new_graph(vec_graph.size());
    for (size_t i = 0; i < graph.size(); ++i) {
      for (size_t j = 0; j < graph[i].size(); ++j) {
        if (!map_edge_delete[graph[i][j].id]) {
          new_graph[i].emplace_back(graph[i][j].to_ver, graph[i][j].str);
        }
      }
    }
    std::swap(new_graph, vec_graph);
  }

  void HangingGraphToNormalGraph(
      std::vector<std::vector<std::pair<size_t, std::string>>

      >& graph,
      std::vector<std::vector<std::pair<size_t, std::string>>>& graph_reverse,
      std::vector<bool>& vec_finished,
      std::vector<size_t>
      & vec_ind) {
    std::vector<bool> used(id_vertex + 1, false);
    HangingGraphToNormalGraphHelper(root_vertex, graph, graph_reverse, vec_finished, vec_ind, used);
  }

  void HangingGraphToNormalGraphHelper(
      Vertex* vertex,
      std::vector<std::vector<std::pair<size_t, std::string>>

      >& graph,
      std::vector<std::vector<std::pair<size_t, std::string>>>& graph_reverse,
      std::vector<bool>& vec_finished,
      std::vector<size_t>
      & vec_ind,
      std::vector<bool>& used
  ) {
    used[vertex->id] = true;
    vec_ind.push_back(vertex->id);
    vec_finished[vertex->id] = vertex->finished;
    for (auto&& it : vertex->vec_edge) {
      graph[vertex->id].emplace_back(it.first->id, it.second);
      graph_reverse[it.first->id].emplace_back(vertex->id, it.second);
      if (!used[it.first->id]) {
        HangingGraphToNormalGraphHelper(it.first, graph, graph_reverse, vec_finished, vec_ind, used);
      }
    }
  }

  static void TopSort(std::vector<bool>& used,
                      std::vector<std::vector<std::pair<size_t, std::string>>

                      >& graph,
                      size_t vertex, std::vector<size_t>
                      & top_sort) {
    used[vertex] = true;
    for (size_t i = 0; i < size_t(graph[vertex].size()); ++i) {
      if (!used[graph[vertex][i].first] && graph[vertex][i].second == "@") {
        TopSort(used, graph, graph[vertex][i].first, top_sort);
      }
    }
    top_sort.push_back(vertex);
  }

  static void GraphCondensation(std::vector<size_t>& ans,
                                std::vector<std::vector<std::pair<size_t, std::string>>

                                >& graph_reverse,
                                size_t vertex, size_t
                                & comp) {
    ans[vertex] = comp;
    for (size_t i = 0; i < size_t(graph_reverse[vertex].size()); ++i) {
      if (ans[graph_reverse[vertex][i].first] == 0U && graph_reverse[vertex][i].second == "@") {
        GraphCondensation(ans, graph_reverse, graph_reverse[vertex][i].first, comp);
      }
    }
  }

  void DfsReordering(Vertex* root) {
    std::vector<bool> used(id_vertex + 1, false);
    size_t counter = 0;
    DfsReorderingHelper(root, counter, used);

  }

  void DfsReorderingHelper(Vertex* vertex, size_t& counter, std::vector<bool>& used) {
    used[vertex->id] = true;
    vertex->vec_ind = counter++;
    for (auto&& it : vertex->vec_edge) {
      if (!used[it.first->id]) {
        DfsReorderingHelper(it.first, counter, used);
      }
    }
  }

  AutomaticMachine PlusAutomaticMachine(AutomaticMachine& first, AutomaticMachine& second) {
    auto* new_root = new Vertex{id_vertex, false, {0},
                                {{first.root_vertex, "@"},
                                 {second.root_vertex, "@"}}};
    first.root_vertex = nullptr;
    second.root_vertex = nullptr;
    return new_root;
  }

  AutomaticMachine MultiplicationAutomaticMachine(AutomaticMachine& first,
                                                  AutomaticMachine& second) {
    std::vector<bool> used(id_vertex + 1, false);
    DfsHelperToMultiplicationAutomaticMachineHelper(first.root_vertex,
                                                    second.root_vertex,
                                                    used);
    auto* new_root = first.root_vertex;
    first.root_vertex = nullptr;
    second.root_vertex = nullptr;
    return new_root;
  }

  void DfsHelperToMultiplicationAutomaticMachineHelper(Vertex* vertex,
                                                       Vertex* start_vertex_other,
                                                       std::vector<bool>& used) {
    used[vertex->id] = true;
    for (auto&& it : vertex->vec_edge) {
      if (!used[it.first->id]) {
        DfsHelperToMultiplicationAutomaticMachineHelper(it.first, start_vertex_other, used);
      }
    }
    if (vertex->finished) {
      vertex->finished = false;
      vertex->vec_edge.emplace_back(start_vertex_other, "@");
    }
  }

  AutomaticMachine StarKliniAutomaticMachine(AutomaticMachine& first) {
    auto* new_root = new Vertex{id_vertex, true, 0, {{first.root_vertex, "@"}}};
    std::vector<bool> used(id_vertex + 1, false);
    DfsHelperToMultiplicationAutomaticMachineHelper(first.root_vertex, new_root, used);
    first.root_vertex = nullptr;
    return new_root;
  }

  AutomaticMachine PlusKliniAutomaticMachine(AutomaticMachine& first) {
    std::vector<bool> used(id_vertex + 1, false);
    DfsHelperToPlusKliniAutomaticMachineHelper(
        first.root_vertex, first.root_vertex, used);
    auto* new_root = first.root_vertex;
    first.root_vertex = nullptr;
    return new_root;
  }

  void DfsHelperToPlusKliniAutomaticMachineHelper(Vertex* vertex,
                                                  Vertex* start_vertex_other,
                                                  std::vector<bool>& used) {
    used[vertex->id] = true;
    for (auto&& it : vertex->vec_edge) {
      if (!used[it.first->id]) {
        DfsHelperToPlusKliniAutomaticMachineHelper(it.first, start_vertex_other, used);
      }
    }
    if (vertex->finished) {
      vertex->vec_edge.emplace_back(start_vertex_other, "@");
    }
  }

  void Folding(std::vector<AutomaticMachine>& stack_token,
               std::vector<char>& stack_operator) {
    if (stack_operator.back() == '+') {
      auto res = PlusAutomaticMachine(
          stack_token[stack_token.size() - 2], stack_token.back());
      stack_token.pop_back();
      stack_token.pop_back();
      stack_token.push_back(res);
    }
    if (stack_operator.back() == '$') {
      auto res = MultiplicationAutomaticMachine(
          stack_token[stack_token.size() - 2], stack_token.back());
      stack_token.pop_back();
      stack_token.pop_back();
      stack_token.push_back(res);
    }
    if (stack_operator.back() == '*') {
      auto res = StarKliniAutomaticMachine(stack_token.back());
      stack_token.pop_back();
      stack_token.push_back(res);
    }
    if (stack_operator.back() == '^') {
      auto res = PlusKliniAutomaticMachine(stack_token.back());
      stack_token.pop_back();
      stack_token.push_back(res);
    }
    stack_operator.pop_back();
    DfsReordering(stack_token.back().root_vertex);
  }

  void StackFolding(std::vector<AutomaticMachine>& stack_token,
                    std::vector<char>& stack_operator, char last_operator) {
    bool flag = false;
    if (last_operator == ')') {
      flag = true;
    }
    while (!stack_operator.empty() && stack_operator.back() != '(' &&
           (map_priority[stack_operator.back()] >=
            map_priority[last_operator] || last_operator == ')')) {
      Folding(stack_token, stack_operator);
    }
    if (!stack_operator.empty() && flag && stack_operator.back() == '(') {
      stack_operator.pop_back();
    }
    if (last_operator != ')') {
      stack_operator.push_back(last_operator);
    }
  }

  void StackFinalFolding(std::vector<AutomaticMachine>& stack_token,
                         std::vector<char>& stack_operator) {
    while (!stack_operator.empty() && stack_operator.back() != '(') {
      Folding(stack_token, stack_operator);
    }
    if (!stack_operator.empty() && stack_operator.back() == '(') {
      stack_operator.pop_back();
    }
  }

  void PresentationHelper(Vertex* vertex, std::vector<bool>& used) {
    used[vertex->id] = true;
    std::cout << vertex->vec_ind;
    if (vertex->finished) {
      std::cout << " 1";
    } else {
      std::cout << " 0";
    }
    std::cout << ":";
    for (auto&& it : vertex->vec_edge) {
      std::cout << it.first->vec_ind << it.second << " ";
    }
    std::cout << std::endl;
    for (auto&& it : vertex->vec_edge) {
      if (!used[it.first->id]) {
        PresentationHelper(it.first, used);
      }
    }
  }

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
