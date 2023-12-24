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

void BroadcastModule::receive_pulse(bool pulse, const string& source,
                                    PulseQueue& pulse_queue) {
  for (const auto& destination : destinations) {
    pulse_queue.push({this->name, destination, pulse});
  }
}

void run_simulation(unordered_map<string, unique_ptr<Module>>& modules,
                    queue<pair<string, bool>>& pulse_queue) {
  auto modules_it = modules.find("broadcaster");
  if (modules_it == modules.end()) {
    throw runtime_error("No broadcaster module found");
  }

  auto destinations = modules_it->second->destinations;
  for (const auto& destination : destinations) {
    pulse_queue.push({destination, true});
  }

  //  now process everything in queue
  std::string source = "";
}