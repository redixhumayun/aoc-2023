#include "puzzle-20.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include "debug.h"

enum ModuleType { FlipFlop, Conjunction, Broadcaster };

void parse_input_file(const string& filename,
                      unordered_map<string, unique_ptr<Module>>& modules) {
  ifstream file(filename);
  string line;

  while (getline(file, line)) {
    stringstream ss(line);
    string module_name, arrow, dest;
    ss >> module_name;

    // Determine the module type and create it if not already created
    ModuleType type;
    if (module_name[0] == '%') {
      type = FlipFlop;
      module_name.erase(0, 1);  // Remove the prefix
    } else if (module_name[0] == '&') {
      type = Conjunction;
      module_name.erase(0, 1);  // Remove the prefix
    } else if (module_name == "broadcaster") {
      type = Broadcaster;
    }

    if (modules.find(module_name) == modules.end()) {
      // Create the module based on its type
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

    // Parse the destinations
    ss >> arrow;  // Read the "->"
    while (ss >> dest) {
      if (dest.back() == ',') dest.pop_back();  // Remove trailing comma
      modules[module_name]->destinations.push_back(dest);

      // For conjunction modules, initialize last_pulse for each input
      if (type == Conjunction) {
        auto& conjunction_module =
            dynamic_cast<ConjunctionModule&>(*modules[module_name]);
        conjunction_module.last_pulse[dest] = false;
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
  }
}