#include "normolized_automatic_machine.h"

bool NormolizedAutomaticMachine::DfsOnString(size_t vertex, std::string str) {
  if (str.empty()) {
    return vec_finished[vertex];
  }
  for (auto&& edge : graph[vertex]) {
    if (edge.second[0] == str[0]) {
      str.erase(str.begin());
      return DfsOnString(edge.first, str);
    }
  }
  return false;
}


void NormolizedAutomaticMachine::Presintation() {
  for (size_t i = 0; i < graph.size(); ++i) {
    std::cout << i << " " << vec_finished[i] << ":";
    for (auto&& it : graph[i]) {
      std::cout << it.first << it.second << " ";
    }
    std::cout << std::endl;
  }
}

void NormolizedAutomaticMachine::DeleteTraslationWithMoreOneWord() {
  for (size_t i = 0; i < graph.size(); ++i) {
    for (auto&& edge : graph[i]) {
      if (edge.second.size() == 1) {
        continue;
      }
      auto temp_edge = edge;
      while (temp_edge.second.size() > 1) {
        graph.push_back({{temp_edge.first, std::string(1, temp_edge.second.back())}});
        temp_edge.second.pop_back();
        temp_edge.first = graph.size() - 1;
      }
      graph[i].emplace_back(temp_edge.first, temp_edge.second);
    }
  }
  for (auto& vec_edge : graph) {
    for (long long j = 0; j < vec_edge.size(); ++j) {
      if (vec_edge[j].second.size() > 1) {
        std::swap(vec_edge[j], vec_edge.back());
        vec_edge.pop_back();
        --j;
      }
    }
  }
}

void NormolizedAutomaticMachine::AlgoTompson() {
  std::queue<std::set<size_t>> queue_set_vertex;
  queue_set_vertex.push({0});
  std::vector<std::vector<std::pair<size_t, std::string>>> new_graph;
  std::vector<bool> new_vec_finished;
  std::map<std::set<size_t>, size_t> map_new_ind_vertex;
  std::set<size_t> set_used;
  map_new_ind_vertex[{0}] = new_graph.size();
  new_vec_finished.push_back(vec_finished[0]);
  new_graph.emplace_back();
  while (!queue_set_vertex.empty()) {
    auto temp_set = queue_set_vertex.front();
    queue_set_vertex.pop();
    if (set_used.contains(map_new_ind_vertex[temp_set])) {
      continue;
    }
    set_used.emplace(map_new_ind_vertex[temp_set]);
    std::map<std::string, std::set<size_t>> map_set_vertex;
    for (auto&& vertex : temp_set) {
      for (auto&& edge : graph[vertex]) {
        map_set_vertex[edge.second].emplace(edge.first);
      }
    }
    for (auto&& it : map_set_vertex) {
      if (!map_new_ind_vertex.contains(it.second)) {
        map_new_ind_vertex[it.second] = new_graph.size();
        bool flag = false;
        for (auto&& vertex : it.second) {
          flag |= vec_finished[vertex];
        }
        new_vec_finished.push_back(flag);
        new_graph.emplace_back();
        queue_set_vertex.emplace(it.second);
      }
      new_graph[map_new_ind_vertex[temp_set]].emplace_back(map_new_ind_vertex[it.second], it.first);
    }
  }
  std::swap(graph, new_graph);
  std::swap(vec_finished, new_vec_finished);
}

void NormolizedAutomaticMachine::BuildPDKA(const std::vector<std::string>& vec_alfabet) { // DKA to PDKA
  bool flag_create_fake_node = false;
  for (size_t i = 0; i < graph.size(); ++i) {
    std::set<std::string> set_translation;
    for (auto&& it : vec_alfabet) {
      set_translation.insert(it);
    }
    for (auto&& it : graph[i]) {
      set_translation.erase(it.second);
    }
    if (!set_translation.empty()) {
      if (!flag_create_fake_node) {
        flag_create_fake_node = true;
        graph.emplace_back();
        vec_finished.push_back(false);
        for (auto&& it : vec_alfabet) {
          graph.back().emplace_back(graph.size() - 1, it);
        }
      }
      for (auto&& it : set_translation) {
        graph[i].emplace_back(graph.size() - 1, it);
      }
    }
  }
}

void NormolizedAutomaticMachine::SupplementPDKA() { // PDKA to Supplement PDKA
  for (bool it : vec_finished) {
    it = !it;
  }
}

void NormolizedAutomaticMachine::BuildMinPDKAByPDKA() { // PDKA to MinPDKA
  std::vector<size_t> color;
  for (bool it : vec_finished) {
    color.push_back(it);
  }
  size_t previous_count_color = 0;
  while (true) {
    std::set<size_t> set_color;
    for (auto&& it : color) {
      set_color.insert(it);
    }
    size_t count_color = set_color.size();
    if (count_color == previous_count_color) {
      break;
    }
    std::vector<std::set<std::pair<size_t, size_t>>> set_translations_out_of_vertex(graph.size());
    std::map<std::string, size_t> map_char_to_uniq_ind;
    size_t uniq_ind = 1;
    for (size_t i = 0; i < graph.size(); ++i) {
      set_translations_out_of_vertex[i].emplace(color[i], 0);
      for (auto&& edge : graph[i]) {
        if (!map_char_to_uniq_ind.contains(edge.second)) {
          map_char_to_uniq_ind[edge.second] = uniq_ind++;
        }
        set_translations_out_of_vertex[i].emplace(
            color[edge.first], map_char_to_uniq_ind[edge.second]);
      }
    }
    size_t uniq_ind_set = 0;
    std::map<std::set<std::pair<size_t, size_t>>, size_t> map_set_to_ind;
    for (size_t i = 0; i < graph.size(); ++i) {
      if (!map_set_to_ind.contains(set_translations_out_of_vertex[i])) {
        map_set_to_ind[set_translations_out_of_vertex[i]] = uniq_ind_set++;
      }
      color[i] = map_set_to_ind[set_translations_out_of_vertex[i]];
    }
    previous_count_color = count_color;
  }
  std::vector<std::vector<std::pair<size_t, std::string>>> new_graph(previous_count_color);
  std::vector<bool> new_vec_finished(previous_count_color);
  std::vector<std::set<std::pair<size_t, std::string>>> set_graph(previous_count_color);
  for (size_t i = 0; i < graph.size(); ++i) {
    if (vec_finished[i]) {
      new_vec_finished[color[i]] = true;
    }
    for (auto&& edge : graph[i]) {
      if (!set_graph[color[i]].contains({color[edge.first], edge.second})) {
        set_graph[color[i]].emplace(color[edge.first], edge.second);
        new_graph[color[i]].emplace_back(color[edge.first], edge.second);
      }
    }
  }
  std::swap(new_graph, graph);
  std::swap(new_vec_finished, vec_finished);
}

size_t NormolizedAutomaticMachine::GetCountVertex() {
  return graph.size();
}

std::string NormolizedAutomaticMachine::CreateRegularExpByDKA() {
  graph.emplace_back();
  vec_finished.emplace_back(true);
  for (size_t i = 0; i < graph.size() - 1; ++i) {
    if (vec_finished[i]) {
      vec_finished[i] = false;
      graph[i].emplace_back(graph.size() - 1, "@");
    }
  }
  std::vector<std::vector<std::pair<size_t, std::string>>> reverse_graph(graph.size());
  for (size_t i = 0; i < graph.size(); ++i) {
    for (auto&& edge : graph[i]) {
      reverse_graph[edge.first].emplace_back(i, edge.second);
    }
  }
  std::set<size_t> set_delete_vertex;
  while (graph.size() - set_delete_vertex.size() > 2) {
    size_t ind_temp_vertex = 1;
    size_t min_value_degree = 1e10;
    for (size_t i = 1; i < graph.size() - 1; ++i) {
      if (!set_delete_vertex.contains(i)) {
        if (graph[i].size() * reverse_graph[i].size() < min_value_degree) {
          min_value_degree = graph[i].size() * reverse_graph[i].size();
          ind_temp_vertex = i;
        }
      }
    }
    set_delete_vertex.insert(ind_temp_vertex);
    std::string str_cycle = "(";
    for (auto&& edge : graph[ind_temp_vertex]) {
      if (edge.first == ind_temp_vertex) {
        if (str_cycle.size() != 1) {
          str_cycle += "+";
        }
        if (edge.second == "@") {
          str_cycle += "1";
        } else {
          str_cycle += edge.second;
        }
      }
    }
    if (str_cycle.size() == 1) {
      str_cycle.pop_back();
    } else {
      str_cycle += ")*";
    }
    for (auto&& edge_to : graph[ind_temp_vertex]) {
      if (set_delete_vertex.contains(edge_to.first)) {
        continue;
      }
      for (auto&& edge_in : reverse_graph[ind_temp_vertex]) {
        if (set_delete_vertex.contains(edge_in.first)) {
          continue;
        }
        if (str_cycle.empty()) {
          std::string edge_str;
          if (edge_in.second != "@") {
            edge_str = edge_in.second;
          }
          if (edge_to.second != "@") {
            if (!edge_str.empty()) {
              edge_str += "$";
            }
            edge_str += edge_to.second;
          }
          graph[edge_in.first].emplace_back(
              edge_to.first, edge_str);
          reverse_graph[edge_to.first].emplace_back(
              edge_in.first, edge_str);
        } else {
          std::string edge_str;
          if (edge_in.second != "@") {
            edge_str = edge_in.second;
          }
          if (str_cycle != "@") {
            if (!edge_str.empty()) {
              edge_str += "$";
            }
            edge_str += str_cycle;
          }
          if (edge_to.second != "@") {
            if (!edge_str.empty()) {
              edge_str += "$";
            }
            edge_str += edge_to.second;
          }
          graph[edge_in.first].emplace_back(
              edge_to.first, edge_str);
          reverse_graph[edge_to.first].emplace_back(
              edge_in.first, edge_str);
        }
      }
    }
    ++ind_temp_vertex;
  }
  std::string cycle_str;
  std::string result;
  for (auto&& edge : graph[0]) {
    if (edge.first == 0) {
      cycle_str += edge.second;
      cycle_str += "+";
    }
    if (edge.first == graph.size() - 1) {
      if (edge.second == "@") {
        result += "1";
      } else {
        result += edge.second;
      }
      result += "+";
    }
  }
  if (!result.empty()) {
    result.pop_back();
  }
  if (!cycle_str.empty()) {
    cycle_str.pop_back();
    if (result.empty()) {
      return "(" + cycle_str + ")*";
    }
    return "(" + cycle_str + ")*$(" + result + ")";
  }
  return result;
};