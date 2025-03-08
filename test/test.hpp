#pragma once
#include <string>
#include <iostream>

inline auto test(const std::string& name){
  std::cerr << "\nTEST:" << name << "\n\n";
}

template<typename T>
inline auto expect(const std::string& test_name, T value, T expected){
  if (value != expected){
    std::cerr << "\nTEST " << test_name << " FAILED!\n";
    std::cerr << "Expected " << expected << ", but got " << value << "\n\n";
    throw std::runtime_error("TEST FAILED!\n");
  }

  std::cerr << "\tTEST " << test_name << " PASSED!\n";
}
