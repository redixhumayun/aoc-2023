// debug.h
#pragma once
#include <iostream>
#include <sstream>
#include <string>

template <typename... Args>
void debug_log(const std::string &message, bool new_line, Args &&...args)
{
#ifdef DEBUG
  std::ostringstream stream;
  stream << message;
  (stream << ... << std::forward<Args>(args)); // Fold expression (C++17)
  if (new_line)
  {
    stream << std::endl;
  }
  std::cout << stream.str();
  if (!new_line)
  {
    std::cout.flush();
  }
#endif
}
