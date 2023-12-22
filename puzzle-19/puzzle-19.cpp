#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "debug.h"

using namespace std;

struct Rule {
  std::string condition;
  std::string destination;
};

struct Workflow {
  std::string name;
  std::unordered_map<char, Rule> rules_map;
  std::string default_destination;
};

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
    Rule rule = {.condition = rule_condition, .destination = destination};
    workflow.rules_map[rule_condition[0]] = rule;
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

auto does_rule_condition_pass(const char& op, int rule_value, int part_value)
    -> bool {
  switch (op) {
    case '<':
      return part_value < rule_value;
      break;
    case '>':
      return part_value > rule_value;
      break;
    default:
      std::cerr << "Invalid operator\n";
      break;
  }
  return false;
}

auto process_part_through_workflow(
    std::unordered_map<char, int> parts_ratings,
    std::unordered_map<std::string, Workflow> graph) -> int {
  auto workflow = graph["in"];
  std::string destination = "";
  while (destination != "A" && destination != "R") {
    auto rules_map = workflow.rules_map;
    bool did_rule_pass = true;
    for (const auto& [rule_part, rule] : rules_map) {
      auto part_value_it = parts_ratings.find(rule_part);
      if (part_value_it == parts_ratings.end()) {
        continue;
      }
      auto op = rule.condition[1];
      auto op_value = std::stoi(rule.condition.substr(2));
      if (!does_rule_condition_pass(op, op_value, part_value_it->second)) {
        did_rule_pass = false;
      } else {
        destination = rule.destination;
        if (destination != "A" && destination != "R" &&
            graph.find(destination) == graph.end()) {
          std::cerr << "Invalid destination\n";
          break;
        }
        workflow = graph[destination];
        did_rule_pass = true;
        break;
      }
    }
    if (did_rule_pass == false) {
      destination = workflow.default_destination;
      if (destination != "A" && destination != "R" &&
          graph.find(destination) == graph.end()) {
        std::cerr << "Invalid destination\n";
        break;
      }
      workflow = graph[destination];
    }
  }
  if (destination == "A") {
    //  calculate sum of values of parts_ratings
    int sum = 0;
    for (const auto& [part, rating] : parts_ratings) {
      sum += rating;
    }
    return sum;
  }
  return 0;
}

auto process_parts_through_workflow(
    std::vector<std::unordered_map<char, int>> parts_ratings_list,
    std::unordered_map<std::string, Workflow> graph) -> int {
  int sum = 0;
  for (const auto& parts_ratings : parts_ratings_list) {
    debug_log("", true, "");
    debug_log("Processing the part ", true, "");
    for (const auto& [part, rating] : parts_ratings) {
      debug_log(" ", false, part, "=", rating);
    }
    debug_log("", true, "");
    auto result = process_part_through_workflow(parts_ratings, graph);
    debug_log("The result is ", true, result);
    sum += result;
  }
  debug_log("", true, "");
  return sum;
}

int main() {
  std::ifstream input_file("puzzle-19-input.txt");
  if (!input_file) {
    std::cerr << "Error opening file\n";
    return 1;
  }

  std::unordered_map<std::string, Workflow> graph;

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
    // print out the rules map
    for (const auto& [key, rule] : workflow.rules_map) {
      debug_log("The rule for ", true, key, " is ", rule.condition, ":",
                rule.destination);
    }
    debug_log("The default destination is ", true,
              workflow.default_destination);
    debug_log("", true, "");
  }

  std::vector<std::unordered_map<char, int>> parts_ratings_list;
  while (getline(input_file, line)) {
    if (line.empty()) {
      break;
    }
    auto parts_ratings = get_parts_ratings(line);
    parts_ratings_list.push_back(parts_ratings);
  }

  //  print out the parts ratings
  debug_log("The parts ratings", true, "");
  for (const auto& parts_ratings : parts_ratings_list) {
    for (const auto& [part, rating] : parts_ratings) {
      debug_log(" ", false, part, "=", rating);
    }
    debug_log("", true, "");
  }

  auto sum = process_parts_through_workflow(parts_ratings_list, graph);
  debug_log("The sum is ", true, sum);
}