#include "puzzle-20.hpp"

void FlipFlopModule::receive_pulse(bool pulse, const string& source,
                                   PulseQueue& pulse_queue) {
  if (pulse) {
    //  if pulse is high, do nothing
    return;
  }
  if (this->state == false) {
    this->state = true;
    pulse_queue.push({this->name, destinations.front(), true});
    return;
  }
  this->state = false;
  pulse_queue.push({this->name, destinations.front(), false});
}

string FlipFlopModule::get_state() const { return this->state ? "1" : "0"; }

void ConjunctionModule::receive_pulse(bool pulse, const string& source,
                                      PulseQueue& pulse_queue) {
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
      pulse_queue.push({this->name, destination, false});
    }
    return;
  }
  //  send a high pulse on the queue for all destinations
  for (const auto& destination : destinations) {
    pulse_queue.push({this->name, destination, true});
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
    pulse_queue.push({this->name, destination, pulse});
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

void run_simulation(unordered_map<string, unique_ptr<Module>>& modules,
                    PulseQueue& pulse_queue) {
  auto initial_state = compute_state(modules);
  auto modules_it = modules.find("broadcaster");
  if (modules_it == modules.end()) {
    throw runtime_error("No broadcaster module found");
  }

  auto destinations = modules_it->second->destinations;
  for (const auto& destination : destinations) {
    pulse_queue.push(
        {.source = "broadcaster", .destination = destination, .pulse = true});
  }

  //  now process everything in queue
  while (!pulse_queue.empty()) {
    const auto& [source, destination, pulse] = pulse_queue.front();
    modules[destination]->receive_pulse(pulse, source, pulse_queue);
  }
}