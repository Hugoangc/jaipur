#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits>
#include <iostream>

inline void limparEntrada()
{
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

#endif
