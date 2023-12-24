#include "puzzle-20.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "debug.h"

enum ModuleType { FlipFlop, Conjunction, Broadcaster };

ModuleType determine_module_type(const string& module_name) {
  if (module_name[0] == '%') {
    return FlipFlop;
  } else if (module_name[0] == '&') {
    return Conjunction;
  } else if (module_name == "broadcaster") {
    return Broadcaster;
  }
  return Broadcaster;  // Default, adjust as needed
}

void parse_input_file(const string& filename,
                      unordered_map<string, unique_ptr<Module>>& modules) {
  ifstream file(filename);
  vector<string> lines;
  string line;

  // First Pass: Create all modules
  while (getline(file, line)) {
    stringstream ss(line);
    string module_name;
    ss >> module_name;

    // Remove prefix if any and determine module type
    ModuleType type = determine_module_type(module_name);

    if (type == FlipFlop || type == Conjunction) {
      module_name.erase(0, 1);
    }

    if (modules.find(module_name) == modules.end()) {
      switch (type) {
        case FlipFlop:
          modules[module_name] = make_unique<FlipFlopModule>();
          break;
        case Conjunction:
          modules[module_name] = make_unique<ConjunctionModule>();
          break;
        case Broadcaster:
          modules[module_name] = make_unique<BroadcastModule>();
          break;
      }
    }

    lines.push_back(line);  // Store the line for the second pass
  }

  // Second Pass: Establish relationships
  for (const auto& parsed_line : lines) {
    stringstream ss(parsed_line);
    string module_name, arrow, dest;
    ss >> module_name >> arrow;

    if (module_name != "broadcaster") {
      module_name.erase(0, 1);
    }

    while (ss >> dest) {
      if (dest.back() == ',') dest.pop_back();  // Remove trailing comma
      modules[module_name]->destinations.push_back(dest);

      // For conjunction modules, store inputs and initialize last_pulse
      if (modules.find(dest) != modules.end() &&
          modules[dest]->name == "Conjunction") {
        auto& conjunction_module =
            dynamic_cast<ConjunctionModule&>(*modules[dest]);
        conjunction_module.state[module_name] = false;
      }
    }
  }

  file.close();
}

int main() {
  unordered_map<string, unique_ptr<Module>> modules;
  parse_input_file("puzzle-20-test.txt", modules);
  for (const auto& [string, class_ptr] : modules) {
    debug_log("Module: ", true, string, " Type: ", class_ptr->name);
    debug_log("Destinations are: ", false, "");
    for (const auto& dest : class_ptr->destinations) {
      debug_log(dest, false, " ");
    }
    debug_log("", true, "");
    if (class_ptr->name == "Conjunction") {
      auto& conjunction_module = dynamic_cast<ConjunctionModule&>(*class_ptr);
      debug_log("State is: ", false, "");
      for (const auto& [input, pulse] : conjunction_module.state) {
        debug_log(input, false, " ");
      }
      debug_log("", true, "");
    }
  }
}