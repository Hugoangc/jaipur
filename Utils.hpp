#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits>
#include <iostream>
using namespace std;

inline void limparEntrada()
{
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

#endif
