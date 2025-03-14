#pragma once
#include <string>
#include <iostream>

template<typename Callable>
inline auto test(const std::string& test_name, Callable callable){
  if (!callable()){
    throw std::runtime_error("TEST " + test_name + " FAILED!\n");
  }
}

template<typename T>
auto check(const T& value, const T& expected){
  std::cerr << "Expected: " << expected << "\nGot: " << value << "\n";
}
