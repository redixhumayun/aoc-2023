#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>

#include "debug.h"

using namespace std;

struct Rule {
  char variable;
  char operation;
  int value;
  std::string destination;
};

struct Workflow {
  std::string name;
  std::vector<Rule> rules;
  std::string default_destination;
};

struct Range {
  int low;
  int high;
};

struct BFS_State {
  std::string workflow_name;
  Range xRange, mRange, aRange, sRange;
};

using Graph = std::unordered_map<std::string, Workflow>;

auto update_range(char variable, char operation, int value, Range current_range)
    -> Range {
  switch (operation) {
    case '<':
      current_range.high = value - 1;
      break;
    case '>':
      current_range.low = value + 1;
      break;
  }
  return current_range;
}

auto is_state_valid(const BFS_State& current_state) -> bool {
  return current_state.xRange.low <= current_state.xRange.high &&
         current_state.mRange.low <= current_state.mRange.high &&
         current_state.aRange.low <= current_state.aRange.high &&
         current_state.sRange.low <= current_state.sRange.high;
}

auto bfs(const Graph& graph) -> long long {
  std::queue<BFS_State> q;
  auto start_node = graph.at("in");
  q.push({.workflow_name = start_node.name,
          .xRange = {1, 4000},
          .mRange = {1, 4000},
          .aRange = {1, 4000},
          .sRange = {1, 4000}});

  long long sum = 0;
  while (!q.empty()) {
    auto current_state = q.front();
    q.pop();

    //  check if this state is valid
    if (!is_state_valid(current_state)) {
      continue;
    }

    if (current_state.workflow_name == "R") {
      //  terminal state to ignore
      continue;
    }
    if (current_state.workflow_name == "A") {
      //  calculate the total possible combinations and add it to the overall
      //  sum
      long long combinations =
          (current_state.xRange.high - current_state.xRange.low + 1) *
          (current_state.mRange.high - current_state.mRange.low + 1) *
          (current_state.aRange.high - current_state.aRange.low + 1) *
          (current_state.sRange.high - current_state.sRange.low + 1);
      sum += combinations;
      continue;
    }
    const Workflow& current_workflow = graph.at(current_state.workflow_name);
    for (const auto& rule : current_workflow.rules) {
      Range new_x = (rule.variable == 'x')
                        ? update_range(rule.variable, rule.operation,
                                       rule.value, current_state.xRange)
                        : current_state.xRange;
      Range new_m = (rule.variable == 'm')
                        ? update_range(rule.variable, rule.operation,
                                       rule.value, current_state.mRange)
                        : current_state.mRange;
      Range new_a = (rule.variable == 'a')
                        ? update_range(rule.variable, rule.operation,
                                       rule.value, current_state.aRange)
                        : current_state.aRange;
      Range new_s = (rule.variable == 's')
                        ? update_range(rule.variable, rule.operation,
                                       rule.value, current_state.sRange)
                        : current_state.sRange;
      BFS_State new_state = {.workflow_name = rule.destination,
                             .xRange = new_x,
                             .mRange = new_m,
                             .aRange = new_a,
                             .sRange = new_s};
      q.push(new_state);
    }
    BFS_State new_state_default = {
        .workflow_name = current_workflow.default_destination,
        .xRange = current_state.xRange,
        .mRange = current_state.mRange,
        .aRange = current_state.aRange,
        .sRange = current_state.sRange};
    q.push(new_state_default);
  }
  return sum;
}

auto construct_workflow(std::string line) -> Workflow {
  int first_curly_brace_index = line.find('{');
  int last_curly_brace_index = line.find('}');

  std::string workflow_name = line.substr(0, first_curly_brace_index);
  Workflow workflow = {.name = workflow_name};
  std::string rules =
      line.substr(first_curly_brace_index + 1,
                  last_curly_brace_index - first_curly_brace_index - 1);

  std::istringstream rules_stream(rules);
  std::string rule;
  while (getline(rules_stream, rule, ',')) {
    int colon_pos = rule.find(":");
    if (colon_pos == string::npos) {
      workflow.default_destination = rule;
      continue;
    }
    std::string rule_condition = rule.substr(0, colon_pos);
    std::string destination = rule.substr(colon_pos + 1);
    Rule rule = {.variable = rule_condition[0],
                 .operation = rule_condition[1],
                 .value = std::stoi(rule_condition.substr(2)),
                 .destination = destination};
    workflow.rules.push_back(rule);
  }
  return workflow;
}

auto get_parts_ratings(std::string line) -> std::unordered_map<char, int> {
  std::unordered_map<char, int> parts_ratings;

  int start_pos = line.find('{');
  int end_pos = line.find('}');

  std::string parts_input = line.substr(start_pos + 1, end_pos - start_pos - 1);
  std::string parts;
  std::istringstream parts_stream(parts_input);
  while (getline(parts_stream, parts, ',')) {
    int break_position = parts.find('=');
    char part = parts[0];
    int rating = std::stoi(parts.substr(2));
    parts_ratings[part] = rating;
  }
  return parts_ratings;
}

int main() {
  std::ifstream input_file("puzzle-19-test.txt");
  if (!input_file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  Graph graph;

  std::string line;
  while (getline(input_file, line)) {
    if (line.empty()) {
      break;
    }
    auto workflow = construct_workflow(line);
    graph[workflow.name] = workflow;
  }
  //  print out the workflows
  debug_log("The workflows", true, "");
  for (const auto& [workflow_name, workflow] : graph) {
    debug_log("", true, workflow_name);
    // print out the rules
    for (const auto& rule : workflow.rules) {
      debug_log("The rule is ", true, rule.variable, rule.operation, rule.value,
                ": ", rule.destination);
    }
    debug_log("The default destination is ", true,
              workflow.default_destination);
    debug_log("", true, "");
  }

  auto sum = bfs(graph);
  debug_log("The sum is ", true, sum);
}