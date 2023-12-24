#include "debug.h"
#include "puzzle-20.hpp"

void FlipFlopModule::receive_pulse(bool pulse, const string& source,
                                   PulseQueue& pulse_queue) {
  if (pulse) {
    //  if pulse is high, do nothing
    return;
  }

  //  if the module has no destinations, do nothing
  if (this->destinations.size() == 0) {
    this->state = !this->state ? true : false;
    return;
  }

  if (this->state == false) {
    this->state = true;
    for (const auto& destination : destinations) {
      pulse_queue.push({this->key, destination, true});
    }
    return;
  }
  this->state = false;
  for (const auto& destination : destinations) {
    pulse_queue.push({this->key, destination, false});
  }
}

string FlipFlopModule::get_state() const { return this->state ? "1" : "0"; }

void ConjunctionModule::receive_pulse(bool pulse, const string& source,
                                      PulseQueue& pulse_queue) {
  if (this->state.find(source) == this->state.end()) {
    std::string error_message = "Source not found for the module " + this->key +
                                " where the source is " + source;
    throw runtime_error(error_message);
  }
  this->state[source] = pulse;
  bool all_inputs_high = true;
  for (const auto& [module, module_state] : this->state) {
    if (!module_state) {
      all_inputs_high = false;
    }
  }

  if (all_inputs_high) {
    //  send a low pulse on the queue for all destinations
    for (const auto& destination : destinations) {
      pulse_queue.push({this->key, destination, false});
    }
    return;
  }
  //  send a high pulse on the queue for all destinations
  for (const auto& destination : destinations) {
    pulse_queue.push({this->key, destination, true});
  }
}

string ConjunctionModule::get_state() const {
  string state_string = "";
  for (const auto& [key, value] : this->state) {
    state_string += key + ":" + (value ? "1" : "0") + ";";
  }
  return state_string;
}

void BroadcastModule::receive_pulse(bool pulse, const string& source,
                                    PulseQueue& pulse_queue) {
  for (const auto& destination : destinations) {
    pulse_queue.push({this->key, destination, pulse});
  }
}

string BroadcastModule::get_state() const { return ""; }

auto compute_state(unordered_map<string, unique_ptr<Module>>& modules)
    -> std::unordered_map<std::string, std::string> {
  std::unordered_map<std::string, std::string> state;
  for (const auto& [key, module] : modules) {
    state[key] = module->get_state();
  }
  return state;
}

void run_simulation_helper(unordered_map<string, unique_ptr<Module>>& modules,
                           PulseQueue& pulse_queue, int* low_count,
                           int* high_count) {
  debug_log("**** New Run Start ****", true, "");
  debug_log("Source: ", true, "Button", " Destination: ", "broadcaster",
            " Pulse: ", "0");
  (*low_count)++;  //  this is the button signal being sent to the broadcaster
  auto modules_it = modules.find("broadcaster");
  if (modules_it == modules.end()) {
    throw runtime_error("No broadcaster module found");
  }

  auto destinations = modules_it->second->destinations;
  for (const auto& destination : destinations) {
    pulse_queue.push(
        {.source = "broadcaster", .destination = destination, .pulse = false});
  }

  //  now process everything in queue
  while (!pulse_queue.empty()) {
    const auto& [source, destination, pulse] = pulse_queue.front();
    debug_log("Source: ", true, source, " Destination: ", destination,
              " Pulse: ", pulse ? "1" : "0", "");
    pulse_queue.pop();
    if (pulse) {
      (*high_count)++;
    } else {
      (*low_count)++;
    }
    modules[destination]->receive_pulse(pulse, source, pulse_queue);
  }
  debug_log("**** New Run End ****", true, "");
}

void run_simulation(unordered_map<string, unique_ptr<Module>>& modules,
                    PulseQueue& pulse_queue, int* low_count, int* high_count,
                    int* runs) {
  auto initial_state = compute_state(modules);
  auto current_state = initial_state;
  do {
    run_simulation_helper(modules, pulse_queue, low_count, high_count);
    current_state = compute_state(modules);
    (*runs)++;
  } while (current_state != initial_state);
}