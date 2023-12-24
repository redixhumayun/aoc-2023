#include <gtest/gtest.h>

#include "puzzle-20.hpp"

TEST(FlipFlopModuleTest, ReceiveHighPulse) {
  FlipFlopModule flip_flop_module = FlipFlopModule();
  flip_flop_module.destinations = {"b"};
  flip_flop_module.state = 0;
  unordered_map<string, unique_ptr<Module>> modules;
  std::queue<std::pair<std::string, bool>> pulse_queue;
  flip_flop_module.receive_pulse(true, modules, pulse_queue);

  ASSERT_EQ(flip_flop_module.state, 1);
  ASSERT_EQ(pulse_queue.size(), 1);
  const auto& [destination, pulse_value] = pulse_queue.front();
  ASSERT_EQ(destination, "b");
  ASSERT_EQ(pulse_value, true);
}

TEST(BroadcastModuleTest, PulseForwarding) {
  BroadcastModule broadcast_module = BroadcastModule();
  broadcast_module.destinations = {"moduleA", "moduleB", "moduleC"};
  unordered_map<string, unique_ptr<Module>> modules;
  std::queue<std::pair<std::string, bool>> pulse_queue;
  broadcast_module.receive_pulse(true, modules, pulse_queue);
  ASSERT_EQ(pulse_queue.size(), 3);

  std::vector<string> expected_destinations = {"moduleA", "moduleB", "moduleC"};
  for (const auto& expected_destination : expected_destinations) {
    ASSERT_EQ(pulse_queue.empty(), false);
    const auto& [destination, pulse] = pulse_queue.front();
    ASSERT_EQ(destination, expected_destination);
    ASSERT_EQ(pulse, true);
    pulse_queue.pop();
  }
}