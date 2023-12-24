#include <gtest/gtest.h>

#include "puzzle-20.hpp"

TEST(FlipFlopModuleTest, ReceiveLowPulseWhenTurnedOff) {
  FlipFlopModule flip_flop_module = FlipFlopModule();
  flip_flop_module.name = "a";
  flip_flop_module.destinations = {"b"};
  flip_flop_module.state = false;
  PulseQueue pulse_queue;
  flip_flop_module.receive_pulse(false, "inv", pulse_queue);

  ASSERT_EQ(flip_flop_module.state, true);
  ASSERT_EQ(pulse_queue.size(), 1);
  const auto& [source, destination, pulse_value] = pulse_queue.front();
  ASSERT_EQ(source, flip_flop_module.key);
  ASSERT_EQ(destination, "b");
  ASSERT_EQ(pulse_value, true);
}

TEST(FlipFlopModuleTest, ReceiveLowPulseWhenTurnedOn) {
  FlipFlopModule flip_flop_module = FlipFlopModule();
  flip_flop_module.name = "a";
  flip_flop_module.destinations = {"b"};
  flip_flop_module.state = true;
  PulseQueue pulse_queue;
  flip_flop_module.receive_pulse(false, "inv", pulse_queue);

  ASSERT_EQ(flip_flop_module.state, false);
  ASSERT_EQ(pulse_queue.size(), 1);
  const auto& [source, destination, pulse_value] = pulse_queue.front();
  ASSERT_EQ(source, flip_flop_module.key);
  ASSERT_EQ(destination, "b");
  ASSERT_EQ(pulse_value, false);
}

TEST(ConjunctionModule, ReceiveLowPulseForInputA_SendHighPulse) {
  ConjunctionModule conjunction_module = ConjunctionModule();
  conjunction_module.name = "con";
  conjunction_module.state = {{"A", false}, {"B", false}, {"C", false}};
  conjunction_module.destinations = {"D"};
  PulseQueue pulse_queue;
  conjunction_module.receive_pulse(true, "A", pulse_queue);

  ASSERT_EQ(conjunction_module.state["A"], true);
  ASSERT_EQ(pulse_queue.size(), 1);
  const auto& [source, destination, pulse_value] = pulse_queue.front();
  ASSERT_EQ(source, conjunction_module.key);
  ASSERT_EQ(destination, "D");
  ASSERT_EQ(pulse_value, true);
}

TEST(ConjunctionModule, ReceiveLowPulseForInputA_SendLowPulse) {
  ConjunctionModule conjunction_module = ConjunctionModule();
  conjunction_module.state = {{"A", false}, {"B", true}, {"C", true}};
  conjunction_module.destinations = {"D"};
  PulseQueue pulse_queue;
  conjunction_module.receive_pulse(true, "A", pulse_queue);

  ASSERT_EQ(conjunction_module.state["A"], true);
  ASSERT_EQ(pulse_queue.size(), 1);
  const auto& [source, destination, pulse_value] = pulse_queue.front();
  ASSERT_EQ(source, conjunction_module.key);
  ASSERT_EQ(destination, "D");
  ASSERT_EQ(pulse_value, false);
}

TEST(BroadcastModuleTest, PulseForwarding) {
  BroadcastModule broadcast_module = BroadcastModule();
  broadcast_module.destinations = {"moduleA", "moduleB", "moduleC"};
  PulseQueue pulse_queue;
  broadcast_module.receive_pulse(true, "", pulse_queue);
  ASSERT_EQ(pulse_queue.size(), 3);

  std::vector<string> expected_destinations = {"moduleA", "moduleB", "moduleC"};
  for (const auto& expected_destination : expected_destinations) {
    ASSERT_EQ(pulse_queue.empty(), false);
    const auto& [source, destination, pulse] = pulse_queue.front();
    ASSERT_EQ(source, broadcast_module.key);
    ASSERT_EQ(destination, expected_destination);
    ASSERT_EQ(pulse, true);
    pulse_queue.pop();
  }
}

TEST(ComputeState, ComputeState) {
  unordered_map<string, unique_ptr<Module>> modules;
  modules["a"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["a"]).state = true;
  modules["a"]->destinations = {"c"};

  modules["b"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["b"]).state = false;
  modules["b"]->destinations = {"c"};

  modules["c"] = make_unique<ConjunctionModule>();
  dynamic_cast<ConjunctionModule&>(*modules["c"]).state["a"] = true;
  dynamic_cast<ConjunctionModule&>(*modules["c"]).state["b"] = false;
  modules["c"]->destinations = {"a"};

  auto state = compute_state(modules);
  ASSERT_EQ(state["a"], "1");
  ASSERT_EQ(state["b"], "0");
  ASSERT_EQ(state["c"], "a:1;b:0;");
}

TEST(RunSimulation, SimpleSimulation) {
  unordered_map<string, unique_ptr<Module>> modules;
  modules["a"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["a"]).state = false;

  modules["b"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["b"]).state = false;

  modules["c"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["c"]).state = false;

  modules["broadcaster"] = make_unique<BroadcastModule>();
  modules["broadcaster"]->destinations = {"a", "b", "c"};

  PulseQueue pulse_queue;
  int low_count = 0;
  int high_count = 0;
  int runs = 0;
  run_simulation(modules, pulse_queue, &low_count, &high_count, &runs);

  ASSERT_EQ(low_count, 8);
  ASSERT_EQ(high_count, 0);
  ASSERT_EQ(runs, 2);
}

TEST(RunSimulation, SimpleSimulation2) {
  unordered_map<string, unique_ptr<Module>> modules;

  modules["a"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["a"]).state = false;
  modules["a"]->key = "a";
  modules["a"]->destinations = {"b"};

  modules["b"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["b"]).state = false;
  modules["b"]->key = "b";
  modules["b"]->destinations = {"c"};

  modules["c"] = make_unique<FlipFlopModule>();
  dynamic_cast<FlipFlopModule&>(*modules["c"]).state = false;
  modules["c"]->key = "c";
  modules["c"]->destinations = {"inv"};

  modules["inv"] = make_unique<ConjunctionModule>();
  dynamic_cast<ConjunctionModule&>(*modules["inv"]).state = {{"c", false}};
  modules["inv"]->key = "inv";
  modules["inv"]->destinations = {"a"};

  modules["broadcaster"] = make_unique<BroadcastModule>();
  modules["broadcaster"]->key = "broadcaster";
  modules["broadcaster"]->destinations = {"a", "b", "c"};

  PulseQueue pulse_queue;
  int low_count = 0;
  int high_count = 0;
  int runs = 0;
  run_simulation(modules, pulse_queue, &low_count, &high_count, &runs);

  ASSERT_EQ(low_count, 8);
  ASSERT_EQ(high_count, 4);
  ASSERT_EQ(runs, 1);
}