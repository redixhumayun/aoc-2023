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
  ASSERT_EQ(source, flip_flop_module.name);
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
  ASSERT_EQ(source, flip_flop_module.name);
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
  ASSERT_EQ(source, conjunction_module.name);
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
  ASSERT_EQ(source, conjunction_module.name);
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
    ASSERT_EQ(source, broadcast_module.name);
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