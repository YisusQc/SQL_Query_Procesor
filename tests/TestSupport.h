#ifndef TEST_SUPPORT_H
#define TEST_SUPPORT_H

#include <iostream>
#include <string>

namespace TestSupport {

  inline int total = 0;
  inline int passed = 0;
  inline int failed = 0;

  inline void check(
    bool condition,
    const std::string& description
  ) {
    ++total;

    if (condition) {
      ++passed;
      std::cout << "[PASS] " << description << '\n';
    } else {
      ++failed;
      std::cout << "[FAIL] " << description << '\n';
    }
  }

  inline void summary() {
    std::cout << "\n========== RESUMEN DE PRUEBAS ==========\n";
    std::cout << "Total: " << total << '\n';
    std::cout << "Correctas: " << passed << '\n';
    std::cout << "Fallidas: " << failed << '\n';
  }

}

#endif

