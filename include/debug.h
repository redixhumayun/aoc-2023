// debug.h
#pragma once
#include <iomanip>  // For std::fixed and std::setprecision
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>  // For std::is_floating_point

template <typename T>
void formatArgument(std::ostream& stream, const T& arg) {
  if constexpr (std::is_floating_point<T>::value) {
    stream << std::fixed << std::setprecision(6)
           << arg;  // Adjust precision as needed
  } else {
    stream << arg;
  }
}

template <typename... Args>
void debug_log(const std::string& message, bool new_line, Args&&... args) {
#ifdef DEBUG
  std::ostringstream stream;
  stream << message;
  (formatArgument(stream, std::forward<Args>(args)),
   ...);  // Use helper function
  if (new_line) {
    stream << std::endl;
  }
  std::cout << stream.str();
  if (!new_line) {
    std::cout.flush();
  }
#endif
}
