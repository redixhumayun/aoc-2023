#ifndef PUZZLE_20_HPP
#define PUZZLE_20_HPP

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
};

class FlipFlopModule : public Module {
 public:
  FlipFlopModule() { name = "FlipFlop"; }
  bool state = false;
  void receive_pulse(bool pulse, const string& source_module,
                     PulseQueue& pulse_queue) override;
};

class ConjunctionModule : public Module {
 public:
  ConjunctionModule() { name = "Conjunction"; }
  unordered_map<string, bool> state;

  void receive_pulse(bool pulse, const string& source_module,
                     PulseQueue& pulse_queue) override;
};

class BroadcastModule : public Module {
 public:
  BroadcastModule() { name = "Broadcast"; }
  void receive_pulse(bool pulse, const string& source_module,
                     PulseQueue& pulse_queue) override;
};

#endif