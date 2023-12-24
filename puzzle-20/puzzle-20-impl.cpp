#include "puzzle-20.hpp"

void FlipFlopModule::receive_pulse(
    bool pulse, unordered_map<string, unique_ptr<Module>>& modules,
    queue<pair<string, bool>>& pulse_queue) {}

void ConjunctionModule::receive_pulse(
    bool pulse, unordered_map<string, unique_ptr<Module>>& modules,
    queue<pair<string, bool>>& pulse_queue) {}

void BroadcastModule::receive_pulse(
    bool pulse, unordered_map<string, unique_ptr<Module>>& modules,
    queue<pair<string, bool>>& pulse_queue) {
  for (const auto& destination : destinations) {
    pulse_queue.push({destination, pulse});
  }
}