#ifndef PUZZLE_20_HPP
#define PUZZLE_20_HPP

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct PulseInfo {
  std::string source;
  std::string destination;
  bool pulse;
};

using PulseQueue = queue<PulseInfo>;

class Module {
 public:
  string name;
  vector<string> destinations;

  virtual ~Module() = default;
  virtual void receive_pulse(bool pulse, const string& source_module,
                             PulseQueue& pulse_queue) = 0;
  virtual string get_state() const = 0;
};

class FlipFlopModule : public Module {
 public:
  FlipFlopModule() { name = "FlipFlop"; }
  bool state = false;
  void receive_pulse(bool pulse, const string& source_module,
                     PulseQueue& pulse_queue) override;
  string get_state() const override;
};

class ConjunctionModule : public Module {
 public:
  ConjunctionModule() { name = "Conjunction"; }
  map<string, bool> state;

  void receive_pulse(bool pulse, const string& source_module,
                     PulseQueue& pulse_queue) override;
  string get_state() const override;
};

class BroadcastModule : public Module {
 public:
  BroadcastModule() { name = "Broadcast"; }
  void receive_pulse(bool pulse, const string& source_module,
                     PulseQueue& pulse_queue) override;
  string get_state() const override;
};

auto compute_state(unordered_map<string, unique_ptr<Module>>& modules)
    -> std::unordered_map<std::string, std::string>;

#endif